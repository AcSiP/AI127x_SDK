# UART IAP
This page describe how to update application firmware by UART2.<BR><BR>

<B>0. Use TeraTerm to open the console.</B><BR>

<B>1. Reset target device to lunch the boot loader.</B><BR>

<B>2. Input 'P' to lunch the boot loader main menu.</B><BR>
![Bootloader Menu](IAP__F401_01.PNG)

<B>3. Input '1', Let boot loader wait for receiving binary file for upgrade.</B><BR>

<B>4. Use TeraTerm to send the firmware file with YModem.</B><BR>
![Bootloader download menu](IAP__F401_02.PNG)

<B>5. Wait for file transmission complete.</B><BR>
If it fails, Back to step 1 and retry again.<BR>
![download progress](IAP__F401_03.PNG)

<B>6. Wait for firmware update complete.</B><BR>
![update completed](IAP__F401_04.PNG)

<B>7. Reset target device. Let new firmware working......</B><BR>

<BR>
<BR>
<BR>
Refer to:<BR>
[AN4567](https://my.st.com/resource/en/application_note/dm00161366.pdf  "https://my.st.com/resource/en/application_note/dm00161366.pdf")
