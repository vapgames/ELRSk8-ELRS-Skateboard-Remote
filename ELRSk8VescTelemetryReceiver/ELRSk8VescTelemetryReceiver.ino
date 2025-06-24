#include <VescUart.h>
#include <SimpleKalmanFilter.h>
#include <stdio.h>
#include "crsfTelemetry.h"
#include <Arduino.h>

//REQUIRED LIBRARIES:
//VescUart
//AlfredoCRSF
//SimpleKalmanFilter

//CONFIG////////////////////////////

//#define STM32F103C8
#define XIAOR4M1

//PINS
#ifdef STM32F103C8
  #define VESC_RX PB11
  #define VESC_TX PB10
  #define CRSF_RX PA10
  #define CRSF_TX PA9
  
  HardwareSerial VESCSerial(VESC_RX, VESC_TX);
  HardwareSerial CRSFSerial(CRSF_RX, CRSF_TX);
 #endif

#ifdef XIAOR4M1
  #define VESC_RX D11
  #define VESC_TX D12
  #define CRSF_RX D7
  #define CRSF_TX D6
  
  #define VESCSerial Serial2
  #define CRSFSerial Serial1
#endif


#define KILOMETERS
//#define MILES

float numCells = 12;
float wheelDiameterMM = 102;
float motorPulleyTeeth = 15;
float wheelPulleyTeeth = 40;
int motorMagnets = 14;

//END CONFIG////////////////////////


VescUart VESCUART;
//TELEMETRY DATA
int rpm;
float voltage;
float current;
int power;
float amphour;
float tach;
float distance;
float velocity;
float watthour;
float batpercentage;
float tempEsc;
float tempMotor;
float whkm = 0;
//MAX VALUES
float maxA = 0;
float maxVel = 0;
float maxTemp = 0;
//filters
SimpleKalmanFilter cellVoltageFilter(0.01f, 0.01f, 0.01f);
SimpleKalmanFilter escTempFilter(0.1f, 0.1f, 0.1f);
SimpleKalmanFilter motorTempFilter(0.1f, 0.1f, 0.1f);


void setup() {
  Serial.begin(115200);

  //CRSF SETUP
  CRSFSerial.begin(CRSF_BAUDRATE);
  SetupCRSF(CRSFSerial);
  
  //VESC SETUP
  VESCSerial.begin(115200);
  VESCUART.setSerialPort(&VESCSerial);
}

//VESC DATA FOR REFERENCE
//struct dataPackage {
//  float avgMotorCurrent;
//  float avgInputCurrent;
//  float dutyCycleNow;
//  float rpm;
//  float inpVoltage;
//  float ampHours;
//  float ampHoursCharged;
//  float wattHours;
//  float wattHoursCharged;
//  long tachometer;
//  long tachometerAbs;
//  float tempMosfet;
//  float tempMotor;
//  float pidPos;
//  uint8_t id;
//  mc_fault_code error; 
//};


#ifdef KILOMETERS
  float metersPerMILEKM = 1.0f / 1000.0f;
#else
  float metersPerMILEKM = 1.0f / 1609.34f;
#endif
float wheelDiameterM = wheelDiameterMM / 1000.0f;
float gearingRatio = motorPulleyTeeth / wheelPulleyTeeth;

void loop()
{
  bool gotValues = VESCUART.getVescValues();

  int polePairs = motorMagnets / 2;
  rpm = VESCUART.data.rpm / polePairs;
  voltage = VESCUART.data.inpVoltage;
  current = VESCUART.data.avgInputCurrent;
  power = voltage * current;
  tempEsc = escTempFilter.updateEstimate(VESCUART.data.tempMosfet);
  tempMotor = motorTempFilter.updateEstimate(VESCUART.data.tempMotor);
  amphour = VESCUART.data.ampHours;
  watthour = VESCUART.data.wattHours;
  tach = VESCUART.data.tachometerAbs / (motorMagnets * 3);

  distance = tach * 3.14159265f * metersPerMILEKM * wheelDiameterM * gearingRatio;       // Motor RPM x Pi x (1 / meters in a mile or km) x Wheel diameter x (motor pulley / wheelpulley)
  velocity = rpm * 3.14159265f * (60.0f * metersPerMILEKM) * wheelDiameterM * gearingRatio;  // Motor RPM x Pi x (seconds in a minute / meters in a mile) x Wheel diameter x (motor pulley / wheelpulley)
  //batpercentage = (((voltage - numCells * 3.3) / numCells) + 0.04f) * 100.0f;                     // ((Battery voltage - minimum voltage) / number of cells) x 100

  maxVel = max(maxVel, velocity);
  maxTemp = max(maxTemp, velocity);
  
  float cellVoltage = cellVoltageFilter.updateEstimate(voltage / 12.0f);
  
  //SEND TELEMETRY
  crsf.update();
  sendRxBattery(cellVoltage * 100, velocity * 100, distance * 10, current * 5);
  
  delay(10);
}
