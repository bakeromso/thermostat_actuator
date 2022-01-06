## New firmware for Thermy V3

include/lcd.h & include/lcd.c are copied from [this project](https://github.com/NerdyProjects/sparmatic-zero) which is licensed under GPLv2.

Termy uses the microcontroller Atmel ATMEGA169(P)

## State of development
Turns out, MPLAB Snap uses some proprietary protocol by Microchip, so there is not (yet) a gdb server that can 
connect to it, so general avr-gdb debugging won't work through the Snap debugger. This is the first reason why it 
might be better to switch over to the Olimex JTAG. 
Snap only works with MPLAB IDE X which looks to be the same hot garbage as it was in the past. Upside is there is 
now (or only now I discovered) way to work with avr-gcc, instead of their compiler per see. Installing IDE and 
importing the project in the IDE worked, but when debugging on Qubes-OS, or actually any form of connecting to the 
Snap debugger (which must be done as root, so IDE must be run with sudo...), Snap debugger disconnects. When 
reattaching the device to the right qube, some error is thrown by MPLAB IDE. Could go on to debug this, seems more 
sensible to connect to JTAG on Thermy. Disadvantage is that things need to be soldered. 

Already found out that the 6 pads on the USB side are probably the JTAG, with the pad closest to the battery pins is 
VCC (actually seems more to be VDD), and pad closest to USB is GND (not 0v, it correctly measures a diode between 
0v battery pad and suspected GND jtag pad).

To do, check the while loops in reading the encoder code. Write in readme about using avr-gcc, jtag on windows 


### How to flash

```shell
avrdude -p m169 -c stk500v1 -P /dev/ttyUSB0 -b 19200 -U flash:w:main.hex 
```

To set the high fuse to 0x99 when using ArduinoMEGA as ISP flasher:
```shell
avrdude -p m169 -c stk500v1 -P /dev/ttyUSB0 -b 19200 -U hfuse:w:0x99:m
```

### Modify connector
#### Doesn't work: clean solution
To do things neatly, order a 5 pin micro USB. Not the on as [on Ali](https://aliexpress.com/item/33036984200.html).
because that one has the casing connected to pin 4, instead of exposing pin 4 through a wire.

USB Pinout | Name on connector
--- | --- 
pin 1 | +
pin 2 | D-
pin 3 | D+
pin 4 |  casing
pin 5 | - / GND


#### Frankenstein
Pinout on USB Frankenstein cable:

USB Pinout | Color USB line | Purpose
--- | --- | ---
pin 1 | black | gnd*
pin 2 | blue | SCK
pin 3 | green | MOSI
pin 4 | white | MISO
pin 5 | red | vcc

*note: not the same as 0v connection on battery of the Thermy!


### Hook it up

#### MPLAB Snap JTAG
Unfortunately, this still means you have to use MPLAB X IDE. Lesson, when buying JTAG to debug make sure there is 
support for the platform on that debugger to do JTAG debug operations.<br>
Also, if possible find a debugger that supports openOCD.

Snap pin nr | Snap pin name | Color
--- | --- | --- 
2 | VTG | red
3 | GND | blue
4 | TDO | grey
5 | TCK | yellow
6 | RESET | 
7 | TDI |  purple
8 | TMS | green

First check if it works without reset, otherwise hook it up to USB reset

#### Doesn't work: Olimex JTAG debugger

Olimex pin nr | Olimex pin name | Jumper color | Atmega pin name
--- | --- | --- | ---
1 | VREF | red | VCC
4 | GND  | blue | GND *
5 | TTDI | purple | TDI
7 | TTMS | green  | TMS
9 | TTCK | yellow  | TCK
13 | TTDO | grey | TDO

*note: not 0v of battery!

#### Doesn't work: MPLAB Snap debugger
Well maybe it works, but not on Qubes OS. Keeps resetting etc. <br>
Also, ISP protocol needs to happen from MPLAB X IDE which is crap (no support on AVRdude for MPLAB Snap)
(maybe from [here](https://github.com/MCUdude/MicroCore/blob/9e54f839781ea1826ac388632f4b642b42cc5ade/avr/avrdude.
conf) if you use a different baudrate e.g. 19200. Haven't tried. )


We will be using the AVR ISP interface of the MPLAB Snap debugger.

Snap pin nr | Snap pin name | Color | .
--- | --- | --- | ---
2 | VTG | USB red  |
3 | GND  | USB black  |
4 | MISO | USB white |
5 | SCK | USB blue |
6 | RESET | loose white/orange |
7 | MOSI | USB green  |


#### Arduino Mega as ISP
When using Arduino Mega as ISP, flash with default ArduinoISP program. Don't forget to place a cap around reset and
ground. Then connect as follows.

Arduino Mega pin | usb pin | Color USB line
--- | --- | ---
Arduino pin 10 | to reset*  | orange/white
Arduino pin 51 MOSI  | to pin 3 | green
Arduino pin 50 MISO | to pin 4 | white
Arduino pin 52 SCK | to pin 2 | blue
Arduino GND | to pin 1 | black
Arduino 5v | to pin 5 | red

*this is the loose cable that is soldered to the reset pad on the Thermy, below the usb mini connector


## Debugging with DebugWIRE

The following is unfortunately unsupported for the used AVR chip, so everything in this section is irrelevant for
this project.
Debugging can be done through the reverse engineered Atmel's reverse engineered debug protocol DebugWIRE. This
enables powerful debug features such as breakpoints. I used [dcwbrown's repo](https://github.com/dcwbrown/dwire-debug) for this.

### Setting it up

#### Fuses
Current fuses

fuse | hex
--- | ---
lfuse | `0x52`
hfuse | `0xd9`
efuse | `0xff`
lock |  `0x3f`

Meaning can easily be found [here](https://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega169pa).

According to the [instruction](https://github.com/dcwbrown/dwire-debug) for using DebugWIRE DWEN should be enabled. This fuse is not present. Is it the same as OCDEN? [Here]https://shawnhymel.com/622/quick-tip-reading-fuse-bits-in-an-arduino/) debugWIRE and OCDEN are mentioned as two seperate things, OCDEN being part of JTAG. So DebugWIRE is probably not supported for the microcontroller in the Thermy v3, the ATmega169PA.

#### Hardware connection / wiring

### Using the debugger



## Notes
When facing the rotary part of the TRV, the part that is normally pushing to the radiator valve pin. When that part 
is rotating clockwise, it means the valve is extracting. Counter clockwise rotations means the valve is retracting. 
If however no retraction is taking place, maybe a bit of pressure on the rotating part with your finger might help 
snap back in place and retract.


### Useful links
[Sparmatic-Comet code from Olell](https://github.com/olell/Sparmatic-Comet) <br>
https://github.com/dcwbrown/dwire-debug <br>
https://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega169pa <br>
https://shawnhymel.com/622/quick-tip-reading-fuse-bits-in-an-arduino/ <br>