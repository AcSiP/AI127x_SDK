## Porting with LIS3DH ##
1.Use I2C interface.<br />
2.Let PA7(Pin 11) to be a Interrupt,GPIOA need enable while system go to sleep.<br />
3.Configure MEMS output interrupt signal is Single-Click.<br />
4.You can modify the threshold level of detection according user-trial on different scenario.<br />

5.More information,please reference<br />
[adafruit lis3dh](https://learn.adafruit.com/adafruit-lis3dh-triple-axis-accelerometer-breakout/wiring-and-test)<br />
[ST lis3dh](http://www.st.com/content/ccc/resource/technical/document/application_note/77/ed/e7/e1/28/5a/45/d6/CD00290365.pdf/files/CD00290365.pdf/jcr:content/translations/en.CD00290365.pdf)<br />



# AI127x_SDK
Software development kit with Private LoRa protocol for AI127x module
![Module Picture](https://github.com/AcSiP/AI127x_SDK/blob/master/Readme/AI1276-401H.jpg)


## SDK Download ##
  Use this repository:  https://github.com/AcSiP/AI127x_SDK.git


## Default Console ##
+ Console port: USART2
+ Baud Rate: 38400 
+ Data bits: 8
+ Parity Check: None
+ Stop bits: 1


## Software Development Guide ##
[Product Breif](http://www.acsip.com.tw/index.php?action=download&perm=d&id=29)

+ Main Process
![Main Process Flow](https://github.com/AcSiP/AI127x_SDK/blob/master/Readme/Main_Process_Flow.png)

+ Noramal Master Process
![Noramal Process on MASTER](https://github.com/AcSiP/AI127x_SDK/blob/master/Readme/OnMaster_for_Normal_Process.png)

+ Noramal Slave Process 
![Noramal Process on SLAVE](https://github.com/AcSiP/AI127x_SDK/blob/master/Readme/OnSlave_for_Noraml__Process.png)


## Software Tools / Utilities ##
[Keil](https://www.keil.com/demo/eval/arm.htm)

[MDK for STM32L0 and STM32F0 Installation & Activation](http://www2.keil.com/stmicroelectronics-stm32/mdk)  
    Product Serial Number (PSN) = U1E21-CM9GY-L3G4L

[Tera Term](https://osdn.net/projects/ttssh2/)

[ST-LINK/V2](https://my.st.com/content/my_st_com/en/products/development-tools/hardware-development-tools/development-tool-hardware-for-mcus/debug-hardware-for-mcus/debug-hardware-for-stm32-mcus/st-link-v2.html)

[DfuSe -- USB device firmware upgrade](https://my.st.com/content/my_st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-programmers/stsw-stm32080.html)


## Reference ##
+ SX127x driver from Semtech  
    http://www.semtech.com/apps/filedown/down.php?file=sx12xxDrivers-V2.1.0.zip

+ LoRaWAN software expansion for STM32Cube  
    http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32cube-expansion-software/i-cube-lrwan.html

