#ifndef CRSF_H
#define CRSF_H

#include <Arduino.h>
#include <stdint.h>
//#include "crc8.h"

/*
 * This file is part of Simple TX
 *
 * Simple TX is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Simple TX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

// Basic setup
#define CRSF_MAX_CHANNEL        16
#define CRSF_FRAME_SIZE_MAX     64

// Device address & type, default here
#ifndef RADIO_ADDRESS
#define RADIO_ADDRESS           0xEA
#endif

// #define ADDR_MODULE             0xEE  //  Crossfire transmitter
#define TYPE_CHANNELS           0x16

// Define AUX channel input limite
#define CRSF_DIGITAL_CHANNEL_MIN 172
#define CRSF_DIGITAL_CHANNEL_MAX 1811

const float CRSFMin = CRSF_DIGITAL_CHANNEL_MIN;
const float CRSFMid = (CRSF_DIGITAL_CHANNEL_MIN + CRSF_DIGITAL_CHANNEL_MAX) / 2;
const float CRSFMax = CRSF_DIGITAL_CHANNEL_MAX;

// internal crsf variables
#define CRSF_TIME_NEEDED_PER_FRAME_US   1100 // 700 ms + 400 ms for potential ad-hoc request
//#define CRSF_SERIAL_BAUDRATE                 115200 //low baud for Arduino Nano , the TX module will auto detect baud. 115200/40000
//#define CRSF_SERIAL_BAUDRATE                 420000
//#define CRSF_SERIAL_BAUDRATE                 400000
#define CRSF_TIME_BETWEEN_FRAMES_US     4000 // 4 ms 250Hz
//#define CRSF_TIME_BETWEEN_FRAMES_US     1666 // 1.6 ms 500Hz
#define CRSF_PAYLOAD_OFFSET             offsetof(crsfFrameDef_t, type)
#define CRSF_MSP_RX_BUF_SIZE            128
#define CRSF_MSP_TX_BUF_SIZE            128
#define CRSF_PAYLOAD_SIZE_MAX           60
#define CRSF_PACKET_LENGTH              22
#define CRSF_PACKET_SIZE                26
#define CRSF_FRAME_LENGTH               24 // length of type + payload + crc
#define CRSF_CMD_PACKET_SIZE            8
#define CRSF_MAX_PACKET_LEN 64
#define CRSF_SYNC_BYTE 0XC8

static const unsigned int CRSF_PACKET_TIMEOUT_MS = 100;
static const unsigned int CRSF_FAILSAFE_STAGE1_MS = 300;
    
// ELRS command
#define ELRS_ADDRESS                    0xEE
#define ELRS_BIND_COMMAND               0xFF
#define ELRS_WIFI_COMMAND               0xFE
#define ELRS_PKT_RATE_COMMAND           0x01
#define ELRS_TLM_RATIO_COMMAND          0x02
#define ELRS_SWITCH_MODE_COMMAND        0x03
#define ELRS_MODEL_MATCH_COMMAND        0x04
#define ELRS_POWER_COMMAND              0x06
#define ELRS_BLE_JOYSTIC_COMMAND        17
#define TYPE_SETTINGS_WRITE             0x2D
#define ADDR_RADIO                      0xEA //  Radio Transmitter


enum chan_order
{
    AILERON,
    ELEVATOR,
    THROTTLE,
    RUDDER,
    AUX1, // (CH5)  ARM switch for Expresslrs
    AUX2, // (CH6)  angel / airmode change
    AUX3, // (CH7)  flip after crash
    AUX4, // (CH8)
    AUX5, // (CH9)
    AUX6, // (CH10)
    AUX7, // (CH11)
    AUX8, // (CH12)
};


// ELRS 3.x (ESP8266 based TX module): with thanks to r-u-t-r-A (https://github.com/r-u-t-r-A/STM32-ELRS-Handset/tree/v4.5)
//  1 : Set Lua [Packet Rate]= 0 - 50Hz / 1 - 100Hz Full / 2- 150Hz / 3 - 250Hz / 4 - 333Hz Full / 5 - 500Hz
//  2 : Set Lua [Telem Ratio]= 0 - Std / 1 - Off / 2 - 1:128 / 3 - 1:64 / 4 - 1:32 / 5 - 1:16 / 6 - 1:8 / 7 - 1:4 / 8 - 1:2 / 9 - Race
//  3 : Set Lua [Switch Mode]=0 -> Hybrid;Wide
//  4 : Set Lua [Model Match]=0 -> Off;On
//  5 : Set Lua [TX Power]=0 Submenu
// 6 : Set Lua [Max Power]=0 - 10mW / 1 - 25mW / 2 - 50mW /3 - 100mW/4 - 250mW  // dont force to change, but change after reboot if last power was greater
// 7 : Set Lua [Dynamic]=0 - Off / 1 - Dyn / 2 - AUX9 / 3 - AUX10 / 4 - AUX11 / 5 - AUX12
// 8 : Set Lua [VTX Administrator]=0 Submenu
// 9 : Set Lua [Band]=0 -> Off;A;B;E;F;R;L
// 10:  Set Lua [Channel]=0 -> 1;2;3;4;5;6;7;8
// 11 : Set Lua [Pwr Lvl]=0 -> -;1;2;3;4;5;6;7;8
// 12 : Set Lua [Pitmode]=0 -> Off;On 
// 13 : Set Lua [Send VTx]=0 sending response for [Send VTx] chunk=0 step=2
// 14 : Set Lua [WiFi Connectivity]=0 Submenu
// 15 : Set Lua [Enable WiFi]=0 sending response for [Enable WiFi] chunk=0 step=0
// 16 : Set Lua [Enable Rx WiFi]=0 sending response for [Enable Rx WiFi] chunk=0 step=2
// //17 : Set Lua [BLE Joystick]=0 sending response for [BLE Joystick] chunk=0 step=0  // not on ESP8266??
// //    Set Lua [BLE Joystick]=1 sending response for [BLE Joystick] chunk=0 step=3
// //    Set Lua [BLE Joystick]=2 sending response for [BLE Joystick] chunk=0 step=3
// 17: Set Lua [Bind]=0 -> 

#define PACKED __attribute__((packed))

enum {
    CRSF_FRAME_LENGTH_ADDRESS = 1, // length of ADDRESS field
    CRSF_FRAME_LENGTH_FRAMELENGTH = 1, // length of FRAMELENGTH field
    CRSF_FRAME_LENGTH_TYPE = 1, // length of TYPE field
    CRSF_FRAME_LENGTH_CRC = 1, // length of CRC field
    CRSF_FRAME_LENGTH_TYPE_CRC = 2, // length of TYPE and CRC fields combined
    CRSF_FRAME_LENGTH_EXT_TYPE_CRC = 4, // length of Extended Dest/Origin, TYPE and CRC fields combined
    CRSF_FRAME_LENGTH_NON_PAYLOAD = 4, // combined length of all fields except payload
};

typedef enum
{
    CRSF_FRAMETYPE_GPS = 0x02,
    CRSF_FRAMETYPE_VARIO = 0x07,
    CRSF_FRAMETYPE_BATTERY_SENSOR = 0x08,
    CRSF_FRAMETYPE_BARO_ALTITUDE = 0x09,
    //CRSF_FRAMETYPE_HEARTBEAT = 0x0B,                   //no need to support? (rev07)
    //CRSF_FRAMETYPE_VIDEO_TRANSMITTER = 0x0F,           //no need to support? (rev07)
    CRSF_FRAMETYPE_LINK_STATISTICS = 0x14,
    // CRSF_FRAMETYPE_OPENTX_SYNC = 0x10,               //not in edgeTX
    // CRSF_FRAMETYPE_RADIO_ID = 0x3A,                  //no need to support?
    CRSF_FRAMETYPE_RC_CHANNELS_PACKED = 0x16,
    // CRSF_FRAMETYPE_LINK_RX_ID = 0x1C,                 //no need to support?
    // CRSF_FRAMETYPE_LINK_TX_ID = 0x1D,                 //no need to support?
    CRSF_FRAMETYPE_ATTITUDE = 0x1E,
    // CRSF_FRAMETYPE_FLIGHT_MODE = 0x21,               //no need to support?
  // Extended Header Frames, range: 0x28 to 0x96
    // CRSF_FRAMETYPE_DEVICE_PING = 0x28,               //no "flight controller" needs to know about this
    // CRSF_FRAMETYPE_DEVICE_INFO = 0x29,               //no "flight controller" needs to know about this
    // CRSF_FRAMETYPE_PARAMETER_SETTINGS_ENTRY = 0x2B,  //no "flight controller" needs to know about this
    // CRSF_FRAMETYPE_PARAMETER_READ = 0x2C,            //no "flight controller" needs to know about this
    // CRSF_FRAMETYPE_PARAMETER_WRITE = 0x2D,           //no "flight controller" needs to know about this
    // CRSF_FRAMETYPE_COMMAND = 0x32,                   //no "flight controller" needs to know about this
  // KISS frames
    // CRSF_FRAMETYPE_KISS_REQ  = 0x78,                 //not in edgeTX
    // CRSF_FRAMETYPE_KISS_RESP = 0x79,                 //not in edgeTX
  // MSP commands
    // CRSF_FRAMETYPE_MSP_REQ = 0x7A,                   //not in edgeTX
    // CRSF_FRAMETYPE_MSP_RESP = 0x7B,                  //not in edgeTX
    // CRSF_FRAMETYPE_MSP_WRITE = 0x7C,                 //not in edgeTX
  // Ardupilot frames
    // CRSF_FRAMETYPE_ARDUPILOT_RESP = 0x80,
} crsf_frame_type_e;

typedef enum
{
    CRSF_ADDRESS_BROADCAST = 0x00,
    CRSF_ADDRESS_USB = 0x10,
    CRSF_ADDRESS_TBS_CORE_PNP_PRO = 0x80,
    CRSF_ADDRESS_RESERVED1 = 0x8A,
    CRSF_ADDRESS_CURRENT_SENSOR = 0xC0,
    CRSF_ADDRESS_GPS = 0xC2,
    CRSF_ADDRESS_TBS_BLACKBOX = 0xC4,
    CRSF_ADDRESS_FLIGHT_CONTROLLER = 0xC8,
    CRSF_ADDRESS_RESERVED2 = 0xCA,
    CRSF_ADDRESS_RACE_TAG = 0xCC,
    CRSF_ADDRESS_RADIO_TRANSMITTER = 0xEA,
    CRSF_ADDRESS_CRSF_RECEIVER = 0xEC,
    CRSF_ADDRESS_CRSF_TRANSMITTER = 0xEE,
} crsf_addr_e;


typedef struct crsf_header_s
{
    uint8_t device_addr; // from crsf_addr_e
    uint8_t frame_size;  // counts size after this byte, so it must be the payload size + 2 (type and crc)
    uint8_t type;        // from crsf_frame_type_e
    uint8_t data[0];
} PACKED crsf_header_t;


typedef struct crsfPayloadLinkstatistics_s
{
    uint8_t uplink_RSSI_1;
    uint8_t uplink_RSSI_2;
    uint8_t uplink_Link_quality;
    int8_t uplink_SNR;
    uint8_t active_antenna;
    uint8_t rf_Mode;
    uint8_t uplink_TX_Power;
    uint8_t downlink_RSSI;
    uint8_t downlink_Link_quality;
    int8_t downlink_SNR;
} crsfLinkStatistics_t;

typedef struct crsf_sensor_battery_s
{
    unsigned voltage : 16;  // V * 10 big endian
    unsigned current : 16;  // A * 10 big endian
    unsigned capacity : 24; // mah big endian
    unsigned remaining : 8; // %
} PACKED crsf_sensor_battery_t;

typedef struct crsf_sensor_gps_s
{
    int32_t latitude;   // degree / 10,000,000 big endian
    int32_t longitude;  // degree / 10,000,000 big endian
    uint16_t groundspeed;  // km/h / 10 big endian
    uint16_t heading;   // GPS heading, degree/100 big endian
    uint16_t altitude;  // meters, +1000m big endian
    uint8_t satellites; // satellites
} PACKED crsf_sensor_gps_t;

typedef struct crsf_sensor_vario_s
{
    int16_t verticalspd; // Vertical speed in cm/s, BigEndian
} PACKED crsf_sensor_vario_t;

typedef struct crsf_sensor_baro_altitude_s
{
    uint16_t altitude; // Altitude in decimeters + 10000dm, or Altitude in meters if high bit is set, BigEndian
    int16_t verticalspd;  // Vertical speed in cm/s, BigEndian
} PACKED crsf_sensor_baro_altitude_t;


typedef struct crsf_sensor_attitude_s
{
    uint16_t pitch;  // pitch in radians, BigEndian
    uint16_t roll;  // roll in radians, BigEndian
    uint16_t yaw;  // yaw in radians, BigEndian
} PACKED crsf_sensor_attitude_t;




class CRSF {
private:
    Stream* CRSFSerial;
public:
    void begin(Stream& _CRSFSerial);
    void crsfPrepareDataPacket(uint8_t packet[], int16_t channels[]);
    void crsfPrepareCmdPacket(uint8_t packetCmd[], uint8_t command, uint8_t value);
    void CrsfWritePacket(uint8_t packet[], uint8_t packetLength);

    //TELEM

    void write(uint8_t b);
    void write(const uint8_t *buf, size_t len);
    //void queuePacket(uint8_t addr, uint8_t type, const void *payload, uint8_t len);
    //void writePacket(uint8_t addr, uint8_t type, const void *payload, uint8_t len);
   
    
    void handleSerialIn();
    void handleByteReceived();
    void shiftRxBuffer(uint8_t cnt);
    void processPacketIn(uint8_t len);
    void checkPacketTimeout();
    void checkLinkDown();

    void packetLinkStatistics(const crsf_header_t *p);
    void packetGps(const crsf_header_t *p);
    void packetVario(const crsf_header_t *p);
    void packetBaroAltitude(const crsf_header_t *p);
    void packetAttitude(const crsf_header_t *p);
    void packetBattery(const crsf_header_t *p);
    void packetChannelsPacked(const crsf_header_t *p);
    
    //TELEM
    CRSF();
    uint8_t _rxBuf[CRSF_MAX_PACKET_LEN+3];
    uint8_t _rxBufPos;
    //Crc8 _crc;
    crsfLinkStatistics_t _linkStatistics;
    crsf_sensor_gps_t _gpsSensor;
    crsf_sensor_vario_t _varioSensor;
    crsf_sensor_baro_altitude_t _baroAltitudeSensor;
    crsf_sensor_attitude_t _attitudeSensor;
    crsf_sensor_battery_t _battery;
    
    uint32_t _baud;
    uint32_t _lastReceive;
    uint32_t _lastChannelsPacket;
    bool _linkIsUp;
};





#if !defined(__linux__)
static inline uint16_t htobe16(uint16_t val)
{
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    return val;
#else
    return __builtin_bswap16(val);
#endif
}

static inline uint16_t be16toh(uint16_t val)
{
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    return val;
#else
    return __builtin_bswap16(val);
#endif
}

static inline uint32_t htobe32(uint32_t val)
{
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    return val;
#else
    return __builtin_bswap32(val);
#endif
}

static inline uint32_t be32toh(uint32_t val)
{
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    return val;
#else
    return __builtin_bswap32(val);
#endif
}
#endif

#endif
