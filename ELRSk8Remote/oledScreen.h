#ifndef OLEDSCREEN_H
#define OLEDSCREEN_H

//Library for the OLED Display
#include <Arduino.h>
#include <U8x8lib.h>
#include "batteryGlyphs.h"

//OLED
//U8X8_SSD1306_64X48_ER_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
U8X8_SSD1306_64X32_1F_HW_I2C  u8x8(/* reset=*/ U8X8_PIN_NONE);


enum ScreenModes{
  SCREEN_VOLTAGE_DISTANCE = 0,
  SCREEN_SPEED_CURRENT = 1,
  SCREEN_RC_LINK = 2,
  SCREEN_MAX_MODES = 2,

  SCREEN_CALIBRATION = 100,
};

const int screenTextWidth = 8;
const int screenTextHeight = 2;

class OledScreenMenu
{
public:
    int remoteBatteryPercent = 0;
    float voltage = 0;
    float current = 0;
    float distance = 0;
    float tempEsc = 0;
    float tempMotor = 0;
    float speed = 0;
    float linkQuality = 0;
    float rssi = 0;
    int throttleCalibrate = 0;
    int batteryCalibrate = 0;
    
    void Setup(int _button1Pin, int initialMode, bool useKilometers){
      u8x8.begin();
      u8x8.clear();
      u8x8.setFont(u8x8_font_8x13B_1x2_r);

      button1 = _button1Pin;
      pinMode(button1, INPUT_PULLUP);

      screenMode = initialMode;
      kilometers = useKilometers;
    }

    void Update() {
      //OPTIMIZED SCREEN TEXT UPDATE (~1300us)
      //At 250HZ there's only 2000us available for screen update
      //So I only update one character per frame, at 250hz that's 15 fps
      //ELRS telemetry packets are way slower anyway


      //unsigned long currentMicros = micros();
      
      if(needsClear) {
        for(int i = 0;i < screenTextWidth;++i) {
          screenTextBuf[0][i] = ' ';
          screenTextBuf[1][i] = ' ';
        }
        needsClear = false;
        return;
      }

      char oldChar;
      
      switch(screenMode) {
        case SCREEN_VOLTAGE_DISTANCE:
          oldChar = screenTextBuf[screenTextY][screenTextX];
          //VOLTAGE    
          if(screenTextY == 0) {
              dtostrf(voltage, 1, 2, screenTextBuf[0]);
              screenTextBuf[0][5] = 'V';
          }
          if(screenTextY == 1) {
            dtostrf(distance, 1, 1, screenTextBuf[1]);
            if(kilometers) {
              screenTextBuf[1][5] = 'k';
              screenTextBuf[1][6] = 'm';
            }
            else {
              screenTextBuf[1][5] = 'm';
              screenTextBuf[1][6] = 'i';
            }
            
          }
          
          //DRAW SINGLE SYMBOL PER UPDATE
          if(screenTextX == 7 && screenTextY == 0) {
            //BATTERY
            drawBatteryTile(remoteBatteryPercent, 7, 0);
          }
          else {
            //CHAR
             u8x8.drawGlyph(screenTextX, screenTextY * 2, screenTextBuf[screenTextY][screenTextX]);
          }
          
          screenTextX++;
          if(screenTextX >= screenTextWidth) {
            screenTextX = 0;
            ++screenTextY;
            if(screenTextY >= screenTextHeight) {
              screenTextY = 0;
            }
          }
        break;
        
        case SCREEN_SPEED_CURRENT:
          if(screenTextY == 0) {
              dtostrf(speed, 4, 1, screenTextBuf[0]);
              if(kilometers) {
                screenTextBuf[0][4] = 'k';
                screenTextBuf[0][5] = 'm';
                screenTextBuf[0][6] = '/';
                screenTextBuf[0][7] = 'h';
              }
              else {
                screenTextBuf[0][4] = ' ';
                screenTextBuf[0][5] = 'm';
                screenTextBuf[0][6] = 'p';
                screenTextBuf[0][7] = 'h';
              }
          }
          if(screenTextY == 1) {
            dtostrf(current, 4, 1, screenTextBuf[1]);
            screenTextBuf[1][6] = 'A';
          }
      
          UpdateChar();
      break;
      
        case SCREEN_RC_LINK:
          if(screenTextY == 0) {
              //dtostrf(linkQuality, 1, 0, screenTextBuf[0]);
              itoa((int)linkQuality, screenTextBuf[0], 10);
              screenTextBuf[0][5] = 'L';
              screenTextBuf[0][6] = 'Q';
          }
          if(screenTextY == 1) {
              //dtostrf(rssi, 1, 0, screenTextBuf[1]);
              itoa((int)rssi, screenTextBuf[1], 10);
              screenTextBuf[1][4] = 'r';
              screenTextBuf[1][5] = 's';
              screenTextBuf[1][6] = 's';
              screenTextBuf[1][7] = 'i';
          }
          
          UpdateChar();
        break;
        case SCREEN_CALIBRATION:
          if(screenTextY == 0) {
            //strcpy(screenTextBuf[0], "Calibrating");
            itoa((int)throttleCalibrate, screenTextBuf[0] + 3, 10);
            screenTextBuf[0][0] = 't';
            screenTextBuf[0][1] = 'h';
            screenTextBuf[0][2] = 'r';
          }
          if(screenTextY == 1) {
            //itoa((int)throttleCalibrate, screenTextBuf[1], 10);
            itoa((int)batteryCalibrate, screenTextBuf[1] + 3, 10);
            screenTextBuf[1][0] = 'b';
            screenTextBuf[1][1] = 'a';
            screenTextBuf[1][2] = 't';
          }
          UpdateChar();
        break;
      }
      
      //Serial.println(micros() - currentMicros);
    
      UpdateButton1();
    }

    void UpdateButton1() {
        //BUTTON WITH DEBOUNCE
        int reading = digitalRead(button1);
        if (reading != lastButtonState) {
          lastDebounceTime = millis();
        }
      
        if ((millis() - lastDebounceTime) > debounceDelay) {
          if (reading != buttonState) {
            buttonState = reading;
      
            //SWITCH MODE
            if (buttonState == LOW) {
              ++screenMode;
              needsClear = true;
              if(screenMode > SCREEN_MAX_MODES){
                screenMode = 0;
              }
            }
          }
        }
      
        lastButtonState = reading;
    }
    
private:
    //SCREEN UPDATE
    char screenTextBuf[screenTextHeight][screenTextWidth + 8];
    int screenTextX = 0;
    int screenTextY = 0;
    bool doDrawBattery = false;
    bool kilometers = false;
    
    //SCREEN PAGES
    int screenMode = 0;
    bool needsClear = false;
    
    //BUTTON 
    int button1;
    int buttonState;
    int lastButtonState = HIGH;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 5;


    void UpdateChar() {
      u8x8.drawGlyph(screenTextX, screenTextY * 2, screenTextBuf[screenTextY][screenTextX]);
      screenTextX++;
      if(screenTextX >= screenTextWidth) {
        screenTextX = 0;
        ++screenTextY;
        if(screenTextY >= screenTextHeight) {
          screenTextY = 0;
        }
      }
    }
    
    void drawBatteryTile(int level, int x, int y) {
      int index = level / 10;
      if (index > 9) {
        index = 9;
      }
      else {
        if (index < 0) {
          index = 0;
        }  
      }
      //DRAW 2 8x8 TILES VERTICALLY STACKED
      u8x8.drawTile(x, y, 1, (uint8_t*)(&batteryGlyphs[index])); // Top tile
      u8x8.drawTile(x, y + 1, 1, (uint8_t*)(&batteryGlyphs[index]) + 8); // Bottom tile
    }
};


#endif
