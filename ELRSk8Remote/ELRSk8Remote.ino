/*******************************************************
 *  ELRSK8 Remote - the Express LRS skateboard remote.
 *
 * Developed By Aleksei Abramenko.
 * The software and hardware designs are provided AS IS WITHOUT ANY WARRANTY!
 * GPL Licensed, see LICENSE
 *
 * Use it at your own risk.
 * I'm not responsible for any damage or accidents caused by proper or improper
 *  use or assembly of the device.
 *********************************************************/

#include <stdint.h>
#include <HardwareSerial.h>
#include "userCfg.h"
#include "board.h"
#include "crsf.h"
#include "led.h"
#include "oledScreen.h"

// CRSF Setup
HardwareSerial CRSFSerial(CRSF_RX, CRSF_TX);
#define CRSF_SERIAL_BAUDRATE  400000

CRSF crsf;
uint8_t crsfPacket[CRSF_PACKET_SIZE];
uint8_t crsfCmdPacket[CRSF_CMD_PACKET_SIZE];
int16_t rcChannels[CRSF_MAX_CHANNEL];
unsigned long crsfTime = 0;
unsigned long loopCount = 0;
unsigned long currentMillis = 0;
float throttle = 0;
float remoteBatteryPercent = 0;
float animationTime = 0;
OledScreenMenu oledScreen;

void setup()
{
  analogReadResolution(ADCResolution);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VOLTAGE_READ_PIN, INPUT);
  
  // Disable LED on the Dev Board to save power, configurable
  if ( onBoardLEDDisable )
  {
    #if BOARD == BOARD_XIAOR4M1
      digitalWrite(LED_BUILTIN, HIGH);
    #elif BOARD == BOARD_STM32F103C8
      digitalWrite(LED_BUILTIN, LOW);
    #else
      #error Specify the PIN and behavior for this board's onboard LED.
    #endif    
  }

  //OLED
  int screenMode = SCREEN_VOLTAGE_DISTANCE;
  bool useKilometers = true;
  #ifdef CALIBRATION
    screenMode = SCREEN_CALIBRATION;
  #endif
  #ifdef MILES
    useKilometers = false;
  #endif
  
  oledScreen.Setup(MENU_BUTTON1_PIN, screenMode, useKilometers);
  
  //Draw Initial screen
  for (uint8_t i = 0; i < screenTextWidth * screenTextHeight; i++) {
    oledScreen.Update();
  }

  //LED
  SetupLED(RGBLED_PIN);

  //CRSF SET CHANNELS
  for (uint8_t i = 0; i < CRSF_MAX_CHANNEL; i++) {
      rcChannels[i] = (CRSF_DIGITAL_CHANNEL_MIN + CRSF_DIGITAL_CHANNEL_MAX) / 2;
  }
  //START CRSF Communications
  delay(100);
  CRSFSerial.begin(CRSF_SERIAL_BAUDRATE);
  crsf.begin(CRSFSerial, SERIAL_8N1);
  while (!CRSFSerial) { ; } // wait for this serial port to connect.

  crsfTime = micros();
  
  // SERIAL SETUP - DEBUG
  Serial.begin(115200,SERIAL_8N1);
  while (!Serial) { ; } // wait for serial port to connect.

  #ifdef CALIBRATION
    throttleMid = avgThrottle(100);
    throttleLow = throttleMid;
    throttleHigh = throttleMid;
  #endif
}

float avgThrottle(int samples) {
  float potInput = 0;
  for(int i = 0;i < samples;++i) {
    potInput += (float)analogRead(THROTTLE_PIN);
  }
  potInput /= samples;
  return potInput;
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return constrain((x - in_min) / (in_max - in_min), 0.0f, 1.0f) * (out_max - out_min) + out_min;
}

float mapfloatUnclamped(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) / (in_max - in_min) * (out_max - out_min) + out_min;
}

float lerp(float a, float b, float x)
{ 
  return a + x * (b - a);
}

float clamp(float x, float xMin, float xMax)
{ 
  if(x < xMin) {
    return xMin;
  }
  if(x > xMax) {
    return xMax;
  }
  return x;
}


bool CRSFUpdate() {
  unsigned long currentMicros = micros();
  //INPUT+CRSF - 635us
  if (currentMicros - crsfTime > CRSF_TIME_BETWEEN_FRAMES_US) {
    //Serial.println(currentMicros - crsfTime); //PERFORMANCE CHECK, should be close to 4000 usec
    //THROTTLE INPUT
    float potInput = avgThrottle(throttleSamples);
    
    int CRSFThrottle = CRSFMid;
    if(potInput < throttleMid) {
      CRSFThrottle = round(mapfloat(potInput, throttleLow, throttleMid, CRSFMin, CRSFMid));
    }
    else {
      CRSFThrottle = round(mapfloat(potInput, throttleMid, throttleHigh, CRSFMid, CRSFMax));
    }

    #ifdef CALIBRATION
      rcChannels[AILERON] = CRSFMid; //DO NOT SEND THROTTLE COMMANDS DURING CALIBRATION
    #else
      rcChannels[AILERON] = CRSFThrottle;
    #endif
    
    throttle =  mapfloat(potInput, throttleLow, throttleHigh, -1.0f, 1.0f);
    
    //SEND
    if (loopCount <= 500) {
        //ESTABLISH CONNECTION BY SENDING NORMAL PACKETS
        crsf.crsfPrepareDataPacket(crsfPacket, rcChannels);
        crsf.CrsfWritePacket(crsfPacket, CRSF_PACKET_SIZE);
        loopCount++;
    } else if (loopCount > 500 && loopCount <= 505) {
        crsf.crsfPrepareCmdPacket(crsfCmdPacket, ELRS_PKT_RATE_COMMAND, ELRSpacketRate);
        crsf.CrsfWritePacket(crsfCmdPacket, CRSF_CMD_PACKET_SIZE);
        loopCount++;
    } else if (loopCount > 505 && loopCount <= 510) {
       crsf.crsfPrepareCmdPacket(crsfCmdPacket, ELRS_POWER_COMMAND, ELRSpower);
        crsf.CrsfWritePacket(crsfPacket, CRSF_PACKET_SIZE);
        loopCount++;
    } else if (loopCount > 510 && loopCount <= 515) {
        crsf.crsfPrepareCmdPacket(crsfCmdPacket, ELRS_TLM_RATIO_COMMAND, ELRStelemetryRate);
        crsf.CrsfWritePacket(crsfCmdPacket, CRSF_CMD_PACKET_SIZE);
        loopCount++;
    }
    else {
        //SEND CHANNELS
        crsf.crsfPrepareDataPacket(crsfPacket, rcChannels);
        crsf.CrsfWritePacket(crsfPacket, CRSF_PACKET_SIZE);
    }

    crsfTime = currentMicros;
    return true;
  }
  //Serial.println((micros() - currentMicros));

  return false;
}

void MeasureRemoteBattery() {
  //INTERNAL BATTERY VOLTAGE
  float batteryADC = (float)analogRead(VOLTAGE_READ_PIN);
  float batteryVoltage = mapfloatUnclamped(batteryADC, batteryADCLow, batteryADCHigh, batteryRealVoltageLow, batteryRealVoltageHigh);
  float newPercent = mapfloat(batteryVoltage, 3.2f, 4.2f, 0, 1);
  remoteBatteryPercent = lerp(remoteBatteryPercent, newPercent, 0.1f);
  oledScreen.remoteBatteryPercent = remoteBatteryPercent * 100;
}

void UpdateLed() {

  if(loopCount < 400)
  {
    //BLINK ON START
    LEDColorRGB.r = lerp(255, 0, remoteBatteryPercent);
    LEDColorRGB.g = lerp(0, 255, remoteBatteryPercent);
    LEDColorRGB.b = 0;
    blinkLED(100);
  }
  else {
    if(throttle > - 0.1f) {
      //IDLE FLASH WHITE
      animationTime += 0.01f;
      float anim = clamp(sin(animationTime) * 4 - 3, 0, 1);
      LEDColorRGB.r = lerp(0, 30, anim);
      LEDColorRGB.g = lerp(0, 40, anim);
      LEDColorRGB.b = lerp(0, 30, anim);
      
      LightLed();
    }
    else {
      //BRAKING FLASH RED
      animationTime += (0.005f - 0.015f * throttle) * 4;
      float anim = sin(animationTime) * 0.5f + 0.5f;
      LEDColorRGB.r = lerp(0, 255, anim);
      LEDColorRGB.g = 0;
      LEDColorRGB.b = 0;
      LightLed();
    }
  }

  if(animationTime > 10000.0f) {
    animationTime = 0;
  }
}

unsigned long measureMicros = micros();
int packetsPerSecond = 0;
void loop()
{
    bool crsfUpdated = CRSFUpdate();
    
    if(crsfUpdated)
    {
      ++packetsPerSecond;
      if(micros() - measureMicros > 1000000) {
        //oledScreen.distance = ((float)packetsPerSecond) * 0.1f;
        //Serial.println(packetsPerSecond);
        measureMicros = micros();
        packetsPerSecond = 0;
      }
    }
    
    if(!crsfUpdated) {
      //RECEIVE TELEMETRY
      crsf.handleSerialIn();
    }
    else {
      //DO SCREEN UPDATE ONCE AFTER CRSF UPDATE
      MeasureRemoteBattery();
      UpdateLed();

      //SEND TELEMETRY VALUES TO SCREEN
      oledScreen.voltage = ((float)crsf._battery.voltage) * 0.001f;
      oledScreen.distance = ((float)crsf._battery.capacity) * 0.1f;
      oledScreen.speed = ((float)crsf._battery.current) * 0.001f;
      oledScreen.current = ((float)crsf._battery.remaining) * 0.5f;
      
      //oledScreen.linkQuality = crsf._linkStatistics.uplink_Link_quality;
      //oledScreen.rssi = crsf._linkStatistics.uplink_RSSI_1;
      oledScreen.linkQuality = crsf._linkStatistics.downlink_Link_quality;
      oledScreen.rssi = crsf._linkStatistics.downlink_RSSI;
      oledScreen.Update();
    }


    #ifdef CALIBRATION
      int potInput = analogRead(THROTTLE_PIN);
      throttleLow = min(throttleLow, potInput);
      throttleMid = potInput;
      throttleHigh = max(throttleHigh, potInput);
      int calibrateVoltage = analogRead(VOLTAGE_READ_PIN);

      oledScreen.throttleCalibrate = potInput;
      oledScreen.batteryCalibrate = calibrateVoltage;
      
      Serial.println("------------------");
      Serial.print("int throttleLow = ");
      Serial.print(throttleLow);
      Serial.println(";");
      Serial.print("int throttleMid = ");
      Serial.print(throttleMid);
      Serial.println(";");
      Serial.print("int throttleHigh = ");
      Serial.print(throttleHigh);
      Serial.println(";");
      Serial.print("//battery = ");
      Serial.println(calibrateVoltage);
    #endif
}
