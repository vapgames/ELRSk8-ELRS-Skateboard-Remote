/***************************
 * USER CONFIG
 **************************/

//#define CALIBRATION

// Supported boards
#define BOARD_XIAOR4M1 1
#define BOARD_STM32F103C8 2

// Set this, see board.h for reference 
#define BOARD BOARD_XIAOR4M1

// onboard LED control
bool onBoardLEDDisable = true;

// Run calibration ahead of time to approximate these values
uint16_t throttleLow = 4950;
uint16_t throttleMid = 8320;
uint16_t throttleHigh = 11650;
float batteryRealVoltageLow = 3.82f;
float batteryRealVoltageHigh = 4.2f;
float batteryADCLow = 9780.0f;
float batteryADCHigh = 10460.0f;

// Safe averaging of the throttle response
uint8_t throttleSamples = 4;    // Default of 4 samples to filter sudden spikes in ADC output.

// ELRS Config Options - should be an enum
int ELRSpacketRate = 3;     // 0 - 50Hz / 1 - 100Hz Full / 2- 150Hz / 3 - 250Hz / 4 - 333Hz Full / 5 - 500Hz
int ELRSpower = 0;          // 0 - 10mW / 1 - 25mW / 2 - 50mW /3 - 100mW
uint8_t ELRStelemetryRate = 4;  // 0 - Std / 1 - Off / 2 - 1:128 / 3 - 1:64 / 4 - 1:32 / 5 - 1:16 / 6 - 1:8 / 7 - 1:4 / 8 - 1:2 / 9 - Race

// UI Settings
#define KILOMETERS
//#define MILES

// LED Settings
// TODO

// CSFR Settings
#define RADIO_ADDRESS   0xEA

/***************************
 * END USER CONFIG
 **************************/
