#include <Arduino.h>

/***************************
 * STM32 F103C8 Pin Setup
 * Board: generic stm32f1
 * bluepillf103c8
 * USART support: enabled (generic serial)
 * USB: CDC generic serial
 * upload method: hid bootloader 2.2
 **************************/
#if BOARD == BOARD_STM32F103C8
  #define CRSF_RX PB11
  #define CRSF_TX PB10
  #define OLED_SDA PB7
  #define OLED_SCL PB6
  #define VOLTAGE_READ_PIN PA6
  #define THROTTLE_PIN PA4
  #define MENU_BUTTON1_PIN PB4
  #define RGBLED_PIN PA8
  #define ADCResolution 12

#elif BOARD == BOARD_XIAOR4M1
/***************************
 * XIAO R4M1 Pin Setup -  BN: Arduino UNO R4 Minima
 **************************/
  #define CRSF_RX D7
  #define CRSF_TX D6
  #define CRSFSerial Serial1
  #define OLED_SDA D4
  #define OLED_SCL D5
  #define VOLTAGE_READ_PIN A0
  #define THROTTLE_PIN A1
  #define MENU_BUTTON1_PIN A2
  #define RGBLED_PIN D9
  #define ADCResolution 14
  #define SERIAL_RX D7    // problem here with these pins ...
  #define SERIAL_TX D6   // problem here with these pins ...

#else // abort, compilation not set to a supported board
  #error specify the BOARD in userCFG.h to a supported board.  Or, setup a new board and its pins correctly 
#endif