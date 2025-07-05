# ELRSk8
![combinedPhoto1](https://github.com/user-attachments/assets/0495cbe9-f083-4f36-914d-0858318545ef)

**This readme is under construction, more instructions and schematics are coming!**


ELRSk8 is an open source remote for DIY electric skateboard utilizing the power of [ExpressLRS](https://github.com/ExpressLRS), running on arduino-compatible controllers.

Main features I tried to achieve with this project:

1. Solve the connection and safety issue with off the shelf remotes. Low latency connection, better than NRF or BLE protocols. ELRS seems like the best open source link right now with superior interference and failsafe handling.
2. Durable throttle control with bearings, high reliability sensor and tension adjustment.
3. Ergonomic grip for use with wrist guards and gloves.
4. Basic telemetry
5. Easily sourced components, arduino compatible boards, 3d printed shell.

Code is based on ELRS control implementation from [Simple TX](https://github.com/kkbin505/Arduino-Transmitter-for-ELRS) by @kkbin505 and [AlfredoCRSF](https://github.com/AlfredoSystems/AlfredoCRSF) library by @AlfredoSystems

I've been iterating on this system for over 1 year, and I've been riding using this remote for 6 months. And at this point I'm confident enough in this design to share it.



# Remote:
<details> <summary>Remote parts you'll need (Click to expand)</summary>
  
- ExpressLRS (ELRS) - Currently it is the most reliable opensource link protocol with great interference and failsafe handling. ELRS receivers can be configured as transmitters, and there's a huge selection of different models. You can use any esp8285 based receiver for TX, I recommend "Happymodel ep1 TCXO" as TX and "Happymodel EPW6 TCXO" as RX.
  
![image](https://github.com/user-attachments/assets/2f6a06a7-f4cf-431d-832c-0b5cdd202b7a)


- controller: RA4M1 it's compact and setup is straightforward. Alternatively you can use STM32F103C8T6 Blue pill (but you need to flash bootloader and remove pins to make it fit). [Xiao RA4M1](https://wiki.seeedstudio.com/getting_started_xiao_ra4m1/) OR [Waveshare RA4M1](https://www.waveshare.com/wiki/RA4M1-Zero)
![image](https://github.com/user-attachments/assets/667876ab-f7f0-4501-8a39-f9daa6081046)


- Sensor: SS49E or A1324 linear hall sensor.
  
![image](https://github.com/user-attachments/assets/a841c091-d416-4ca6-862f-5d14742ad6da)
 
- Neodimium magnets 5x3

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
</details>

Remote schematic for XIAO RA4M1 and ND1A05MA
![schematic1](https://github.com/user-attachments/assets/f2669dd6-1ed2-4d2f-ae5b-18d55915165f)
![2025-06-13 13 42 58](https://github.com/user-attachments/assets/3f3e439e-055b-4374-a821-cc2aff2158a5)
![2025-05-21 17 16 38](https://github.com/user-attachments/assets/f1d53784-b000-4700-a6d3-c126b6069abe)



# Receiver:
- Any ELRS receiver will do. But if you want to have telemetry at this point you'll need a PWM receiver. I use Happymodel EPW6, it can do UART and PWM at the same time.

- For telemetry - to talk to VESC you need RA4M1 / STM32F103C8T6 . Or any other arduino-compatible controller that can do **2 UARTS!**

![schematicReceiver1](https://github.com/user-attachments/assets/391bb1f6-6eb4-4b51-ba57-ddfe61a59739)


---
# ELRS Flashing and configuring
- <details> <summary>How to enter ELRS WIFI mode (Click to expand)</summary>
  
  * For me ELRS WIFI doesn't work on my laptop, but it works on my phone.
  
  * By default ELRS receiver/transmitter will go into WIFI mode 60 seconds after power up, if there's no connection.
  
  * For fresh receiver you can power it with GND and +5v and wait 60 seconds.
  
  * If connection was established between remote and the board receiver, it will not go into WIFI mode until power cycle. So to connect WIFI to your board - keep remote turned off, power the board on, wait 60 seconds.
  
  
  * For soldered remote, the TX module will not go into WIFI mode while it's receiving packets from arduino controller. To go into WIFI mode you need to flash empty sketch (like a blinker sketch) onto the remote, then wait 60 seconds.
  
  * Choose WIFI named ExpressLRS RX (or TX). Press "Use network as is":
    
     ![image](https://github.com/user-attachments/assets/61f28bab-d0c6-477a-9c94-dc80ef6c00dd)
  
  
  * After this you can go into a browser and type address 10.0.0.1
  Here you can set your Bind Phrase, tweak pin settings, and upload the firmware (that you download from [web flasher](https://expresslrs.github.io/web-flasher/))

    ![image](https://github.com/user-attachments/assets/0e6a01b5-8cc2-4e8a-9587-3afe17918779) ![image](https://github.com/user-attachments/assets/b1224616-2e5d-4d0c-9835-f61d55d47192)
</details>


- <details> <summary>How to flash Ep1 as TX (Click to expand)</summary>
  
  * Go to https://expresslrs.github.io/web-flasher/
  
  * Choose Receiver:
  
    ![image](https://github.com/user-attachments/assets/65ffad33-0fa8-479d-b1d7-3a2ba975e44e)

  * Choose latest ELRS and these receiver settings:
  
    ![image](https://github.com/user-attachments/assets/274a7e57-05c3-4544-aa0c-65c2035afa71)

  * Set your bind phrase, go to Advanced Settings and check "Flash RX as TX"
  
    ![image](https://github.com/user-attachments/assets/f9781d08-f73f-4233-a813-7e142f5eb5c4)

  * Download firmware and flash using WIFI method OR serial FTDI adapter
  
</details>


- <details> <summary>How to configure EPW6 for PPM and UART telemetry (Click to expand)</summary>
  
  * Go into receiver WIFI mode
    
  * CH1 is throttle PWM, I've increased mine to 100HZ, test this for your particular VESC. Make sure to setup **failsafe pos to 1500**.
    
  * CH2 and CH3 should be Serial TX and RX.
  
    ![image](https://github.com/user-attachments/assets/0f130994-d8fe-4521-87ea-21b3da6e2d6c)

</details>

- <details> <summary>How to flash with FTDI adapter (Click to expand)</summary>
  
  * The "I wasn't asking" method that I prefer over WIFI flashing.
  
  * Bridge the boot pin or press the boot button before powering up before flashing.
  
    ![FTDIFlashing1](https://github.com/user-attachments/assets/e84fd2e5-6c49-4356-8602-25ea1d0dfa93)

  
  * Use flashing method "Serial UART"
  
    ![image](https://github.com/user-attachments/assets/39008df4-8217-4b54-8c27-283a78eb0d7c)

  
  * Select your FTDI serial in popup

    ![image](https://github.com/user-attachments/assets/48ea31e2-5f47-4bd7-9a66-969f36361ed4)

</details>
