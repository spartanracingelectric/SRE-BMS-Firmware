<<<<<<< HEAD
## Rules related to BMS software

#### EV.8.3 Accumulator Management System - AMS

EV.8.3.1 An Accumulator Management System must monitor the Accumulator(s) Voltage EV.8.4 and
Temperature EV.8.5 when the:
</br> &nbsp; &nbsp; &nbsp; &nbsp; a. Tractive System is Active EV.9.1
</br> &nbsp; &nbsp; &nbsp; &nbsp; b. Accumulator is connected to a Charger EV.10.3

EV.8.3.2 Cell balancing when the Accumulator Isolation Relays (AIR) EV.6.4 are open is not permitted.

EV.8.3.3 The AMS must monitor for:
</br> &nbsp; &nbsp; &nbsp; &nbsp; a. Voltage values outside the allowable range EV.8.4.2
</br> &nbsp; &nbsp; &nbsp; &nbsp; b. Voltage sense Overcurrent Protection device(s) blown or tripped
</br> &nbsp; &nbsp; &nbsp; &nbsp; c. Temperature values outside the allowable range EV.8.5.2
</br> &nbsp; &nbsp; &nbsp; &nbsp; d. Missing or interrupted voltage or temperature measurements
</br> &nbsp; &nbsp; &nbsp; &nbsp; e. A fault in the AMS

#### EV.8.4 Accumulator Voltage

EV.8.3.4 If the AMS detects one or more of the conditions of EV.8.3.3 above, the AMS must:
</br> &nbsp; &nbsp; &nbsp; &nbsp; a. Open the Shutdown Circuit EV.8.2.2
</br> &nbsp; &nbsp; &nbsp; &nbsp; b. Turn on the AMS Indicator Light

EV.8.4.2 Cell Voltage levels must remain inside the allowed minimum and maximum cell voltage levels stated in the cell data sheet. Measurement accuracy must be considered.

#### EV.8.5 Accumulator Temperature

EV.8.5.2 Temperatures must remain below the allowed maximum cell temperature limit stated in the cell data sheet or 60°C, whichever is lower. Measurement accuracy must be considered.

## Other parts of this project

[ENNOID-BMS Configuration tool](https://github.com/EnnoidMe/ENNOID-BMS-Tool) the tool to configure the ENNOID-BMS and upload the bootloader / update the main firmware.


When flashing the application the start address should be: <b>0x08000000</b>
When flashing the bootloader the start address should be: <b>0x08032000</b>

The flash is formatted as follows (summary):

((uint32_t)0x08000000) /* Base @ of Page 0, 2 Kbytes */  // Startup Code - Main application<br>
((uint32_t)0x08000800) /* Base @ of Page 1, 2 Kbytes */  // Page0 - EEPROM emulation<br>
((uint32_t)0x08001000) /* Base @ of Page 2, 2 Kbytes */  // Page1 - EEPROM emulation<br>
((uint32_t)0x08001800) /* Base @ of Page 3, 2 Kbytes */  // Remainder of the main application firmware stars from here.<br>
((uint32_t)0x08019000) /* Base @ of Page 50, 2 Kbytes */  // New app firmware base addres<br>
((uint32_t)0x08032000) /* Base @ of Page 100, 2 Kbytes */  // Bootloader base<br>

See "modFlash.h" and "modFlash.c" for more info.
=======
## Rules related to BMS software

#### EV.8.3 Accumulator Management System - AMS

EV.8.3.1 An Accumulator Management System must monitor the Accumulator(s) Voltage EV.8.4 and
Temperature EV.8.5 when the:
</br> &nbsp; &nbsp; &nbsp; &nbsp; a. Tractive System is Active EV.9.1
</br> &nbsp; &nbsp; &nbsp; &nbsp; b. Accumulator is connected to a Charger EV.10.3

EV.8.3.2 Cell balancing when the Accumulator Isolation Relays (AIR) EV.6.4 are open is not permitted.

EV.8.3.3 The AMS must monitor for:
</br> &nbsp; &nbsp; &nbsp; &nbsp; a. Voltage values outside the allowable range EV.8.4.2
</br> &nbsp; &nbsp; &nbsp; &nbsp; b. Voltage sense Overcurrent Protection device(s) blown or tripped
</br> &nbsp; &nbsp; &nbsp; &nbsp; c. Temperature values outside the allowable range EV.8.5.2
</br> &nbsp; &nbsp; &nbsp; &nbsp; d. Missing or interrupted voltage or temperature measurements
</br> &nbsp; &nbsp; &nbsp; &nbsp; e. A fault in the AMS

#### EV.8.4 Accumulator Voltage

EV.8.3.4 If the AMS detects one or more of the conditions of EV.8.3.3 above, the AMS must:
</br> &nbsp; &nbsp; &nbsp; &nbsp; a. Open the Shutdown Circuit EV.8.2.2
</br> &nbsp; &nbsp; &nbsp; &nbsp; b. Turn on the AMS Indicator Light

EV.8.4.2 Cell Voltage levels must remain inside the allowed minimum and maximum cell voltage levels stated in the cell data sheet. Measurement accuracy must be considered.

#### EV.8.5 Accumulator Temperature

EV.8.5.2 Temperatures must remain below the allowed maximum cell temperature limit stated in the cell data sheet or 60°C, whichever is lower. Measurement accuracy must be considered.

## Compiling with ARM-GCC
You can manipulate the makefile to choose which files you want included in the compilation.

#### Windows
1. Install WSL using the Ubuntu distro
2. In the WSL terminal, `sudo apt-get update` and `sudo apt update`
3. `sudo apt install make` and `sudo apt install gcc-arm-none-eabi`
4. Run the makefile in the root directory by running `make`.

#### Ubuntu/Linux Mint
1. In the WSL terminal, `sudo apt-get update` and `sudo apt update`
2. `sudo apt install make` and `sudo apt install gcc-arm-none-eabi`
3. Run the makefile in the root directory by running `make`.

## Compiling with MDK
Ask Sean lol. Licensing = pain

## Other parts of this project

This is the firmware repository containing all firmware source files. There are three more repositories for this project:

[ENNOID-BMS Hardware](https://github.com/EnnoidMe/ENNOID-BMS) The hardware sourcefiles.

[ENNOID-BMS Bootloader](https://github.com/EnnoidMe/DieBieMS-Bootloader) Same as DieBieMS bootloader. Can be flashed with the BMS Tool in the firmware tab or with an STlink-V2. 

[ENNOID-BMS Configuration tool](https://github.com/EnnoidMe/ENNOID-BMS-Tool) the tool to configure the ENNOID-BMS and upload the bootloader / update the main firmware.


When flashing the application the start address should be: <b>0x08000000</b>
When flashing the bootloader the start address should be: <b>0x08032000</b>

The flash is formatted as follows (summary):

((uint32_t)0x08000000) /* Base @ of Page 0, 2 Kbytes */  // Startup Code - Main application<br>
((uint32_t)0x08000800) /* Base @ of Page 1, 2 Kbytes */  // Page0 - EEPROM emulation<br>
((uint32_t)0x08001000) /* Base @ of Page 2, 2 Kbytes */  // Page1 - EEPROM emulation<br>
((uint32_t)0x08001800) /* Base @ of Page 3, 2 Kbytes */  // Remainder of the main application firmware stars from here.<br>
((uint32_t)0x08019000) /* Base @ of Page 50, 2 Kbytes */  // New app firmware base addres<br>
((uint32_t)0x08032000) /* Base @ of Page 100, 2 Kbytes */  // Bootloader base<br>

See "modFlash.h" and "modFlash.c" for more info.
>>>>>>> 23bcaebbb8e0ddb3a91387fa80382be3711e99ee
