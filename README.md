# ELRSk8
![combinedPhoto1](https://github.com/user-attachments/assets/0495cbe9-f083-4f36-914d-0858318545ef)

ELRSk8 is an open source remote for DIY electric skateboard utilizing the power of ExpressLRS.

Main features I tried to achieve with this project:

1. Solve the connection and safety issue with off the shelf remotes. Low latency connection, better than NRF or BLE protocols. ELRS seems like the best open source link right now with superior interference and failsafe handling.
2. Durable throttle control with bearings, high reliability sensor and tension adjustment.
3. Ergonomic grip for use with wrist guards and gloves.
4. Basic telemetry
5. Easily sourced components and 3d printed shell.

Code is based on ELRS control implementation from [Simple TX](https://github.com/kkbin505/Arduino-Transmitter-for-ELRS) by @kkbin505 and [AlfredoCRSF](https://github.com/AlfredoSystems/AlfredoCRSF) library by @AlfredoSystems

I've been iterating on this system for over 1 year, and I've been riding using this remote for 6 months. And at this point I'm confident enough in this design to share it.
This project post is under construction, more instructions and schematics are coming.


---
**Remote config:**
- ExpressLRS (ELRS) - Currently it is the most reliable opensource link protocol with great interference and failsafe handling. ELRS receivers can be configured as transmitters, and there's a huge selection of different models. You can use any esp8285 based receiver for TX, I recommend "Happymodel ep1 TCXO" as TX and "Happymodel EPW6 TCXO" as RX.
  
![image](https://github.com/user-attachments/assets/2f6a06a7-f4cf-431d-832c-0b5cdd202b7a)


- controller: RA4M1 it's compact and setup is straightforward. Alternatively you can use STM32F103C8T6 Blue pill (but you need to flash bootloader and remove pins to make it fit). [Xiao RA4M1](https://wiki.seeedstudio.com/getting_started_xiao_ra4m1/) OR [Waveshare RA4M1](https://www.waveshare.com/wiki/RA4M1-Zero)
![image](https://github.com/user-attachments/assets/667876ab-f7f0-4501-8a39-f9daa6081046)


- Sensor: SS49E or A1324 linear hall sensor.
  
![image](https://github.com/user-attachments/assets/a841c091-d416-4ca6-862f-5d14742ad6da)


- Bearings: MR128 ZZ (or MR128 RS)  8 * 12 * 3.5mm
  
![image](https://github.com/user-attachments/assets/28e9f44e-3933-425b-847e-37416a9172b8)

- Spring from Bic lighter.

- Battery: 802540 or 902540 900mah li-ion spicy pillow (lasts >6 hours, haven't tested beyond that)

![image](https://github.com/user-attachments/assets/aee778e6-1d69-49d4-b16d-e391b49c1871)


- Usb C port: generic pcb soldered 4 pin port.
  
![image](https://github.com/user-attachments/assets/b5586c1c-5f05-47c0-8872-ac2bea37eca7)


- 6x6x7 button for telemetry menu
  
![image](https://github.com/user-attachments/assets/dbcb2181-2e78-4173-9e72-a6c209cf87b7)

- Power switch: KCD1-110
  
![image](https://github.com/user-attachments/assets/1b7bd9e6-8d7a-4733-ac7c-d268f51610c8)

- OLED 0.49" i2c 64x32
  
![image](https://github.com/user-attachments/assets/534a824a-699a-426f-b127-3648d601702b)

- Power supply:

    - 5v UPS module ND1A05MA - combines 0.5A charging and discharging, but seems like it's not very widely available
      
    ![image](https://github.com/user-attachments/assets/c2eb5e0e-282f-4f1b-a126-8b18745eaaba)

     - alternatively you can use 2 separate modules for charging and discharging. Look for 500ma small capacity charging module + 5v boost module
       
     ![image](https://github.com/user-attachments/assets/cc81d290-faf7-4987-a5f2-a1a0bfb038fa)

- 56k resistors x 2. For battery voltage measurement.

- Shell printed with PETG (or any other low friction material). Grips printed with TPU95a.
 
- Hardware 
  - set of m2 screws 6-12mm
    
  - m5x4 or m5x5 set screw (same as in HTD5m pulley)
    
    ![image](https://github.com/user-attachments/assets/36841690-e4ca-4169-bd70-ea382588c499)

- Optional WS2812 LEDs

---
Here's remote schematic for XIAO RA4M1 and ND1A05MA
![schematic1](https://github.com/user-attachments/assets/f2669dd6-1ed2-4d2f-ae5b-18d55915165f)
![2025-06-13 13 42 58](https://github.com/user-attachments/assets/3f3e439e-055b-4374-a821-cc2aff2158a5)

---
Receiver
- Any ELRS receiver will do. But if you want to have telemetry at this point you'll need a PWM receiver. I use Happymodel EPW6, it can do UART and PWM at the same time.

- For telemetry - to talk to VESC you need RA4M1 / STM32F103C8T6 . Or any other arduino-compatible controller that can do **2 UARTS!**

---
![schematicReceiver1](https://github.com/user-attachments/assets/bee59398-9846-46dc-8a53-ecd2bb32b1dc)


