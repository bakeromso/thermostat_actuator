# Radiator Actuator
Note:
This project is part of a larger smart home effort working with Home Assistant. I feel like there should be a better
way to document all the subproject in one place, so that might still come.

## Table of Contents
1. [About this project](#About-this-project)
1. [ESP8266 toolchain SDK installation](#ESP8266-toolchain-SDK-installation)
1. [CLion Instructions](#CLion-Instructions)
1. [WiFi and MQTT configuration with SDKCONFIG](#WiFi-and-MQTT-configuration-with-SDKCONFIG)
1. [Findings on alternative firmware]()
1. [Related links](#Related-links)

## About this project
### Project description
This project intends to upgrade existing 'dumb' electronic radiator valves such that they can be controlled by home automation, such as Home Assistant. The actuator is controlled by an ESP8266 that emulates the rotary encoder on the device. 


### Alternative considerations
Previous work on the topic can be found on the German forum [mikrocontroller](https://www.mikrocontroller.net/articles/Sparmatic_Heizungsthermostate), where many variants of the thermostatic radiator valve are discussed. They are all manufactured by Eurotronic, but sold under different names such as Thermy, Rotheigner, Zero, Thermotronic and Comet. Here, alternative firmwares are suggested too, the best one I found to be by [Ollel](https://www.mikrocontroller.net/articles/Sparmatic_Heizungsthermostate). On my [fork](https://www.mikrocontroller.net/articles/Sparmatic_Heizungsthermostate) I will publish a mod working with ESP8266 for remote controll once I got that working.

The reason I chose not to go for these alternative firmwares in the first place, is that simply simulating the rotary encoder by an ESP8266 seemed an easier way to achieve remote control over the actuator. However, to get more fine grained control over the device, I decided to investigate how to reflash the on board Atmel, see section 

An extremely interesting DIY build was done by [AJStubbsy](https://www.instructables.com/Smart-Radiator-Valve-With-Home-Assistant/), which I intended to use first. Unfortunately, the affordable linear motor they used is no longer available. I couldn't find a first hand alternative, but considered gathering an alternative linear motor at any car repair shop. The linear motor used in side mirrors are a potential well-fitting alternative. Alltogether, this option seemed to be possible but more work (and possibly more expensive) than modding a second hand Thermy.

Thus far all options are based on mechanical actuation, where a linear motor drives the radiator valve. Alternatively, I could have gone for thermal actuators as used in underfloor heating. They operate by heating a wax element that will drive the radiator valve open or closed. In underfloor heating however, response times of minutes are acceptable, but I suspected I might need faster response times (in hindsight, minutes would be acceptable). If I went for this solution, I wanted to go for the more affordable units. However, I felt uncomfortable leaving those uncertified units unattended at home for fire hazard reasons. 
Another disadvantage of these units is that they are discrete actuators. Since there is no temperature sensor embedded, it would be difficult if not impossible to drive the radiator half open. Therefore, these units discard the possibility to go for more fine grained control of the radiator in the future, as would be possible with the motorized units.

## ESP8266 toolchain SDK installation

For clarity, toolchain refers to the `xtensa-lx106-elf-XXX` binaries used to compile your ESP8266 projects with, and 
SDK refers to the libraries/header files and examples from Espressif to make your life as a programmer easier. Also the 
tool to flash and monitor your device is part of the SDK.

Get the latest toolchain from the Github [repository of Espressif](https://github.com/espressif/ESP8266_RTOS_SDK/)
After you've gotten the latest toolchain, follow the remaining instructions on the repository to also install the 
latest SDK. More information about the installation can be found on their [getting started page](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/).



## CLion Instructions

Open **File** > **Settings** > **Build, Execution, Deployment** > **CMake**. Select
 the profile you want to work on (or create if there's none yet). For *CMake options* 
 fill in:
 ```
-DCMAKE_C_COMPILER="/home/user/esp/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc" -DCMAKE_CXX_COMPILER="/home/user/esp/xtensa-lx106-elf/bin/xtensa-lx106-elf-g++"
```
Note: replace with your own absolute paths of where you put the toolchain.

For *Build options* put `-- -j N` where N is the number of processor cores in your system + 1. So for a quad core 
processor N will be 5.

For *Environment* I had to put `ESPPORT=/dev/ttyUSB0` to force the SDK to use the correct USB port. This might
not be necessary in your case. Of course replace with the USB port your device is connected to. The USB port can be 
found on Linux by in a terminal running `ls /dev/ttyUSB?`. If there's multiple results, disconnect your device to find
the difference in result, that being your ESP8266.

## WiFi and MQTT configuration with SDKCONFIG 

Open a terminal in your project directory and run `make menuconfig`. A custom entry has been added thanks to the `Kconfig.projbuild` file in the `main` folder. Here enter the SSID and password of your WiFi. For the MQTT broker URL adhere to the following format `mqtt://[username]:[password]@[broker_url]:[port]`. Here replace with your own username, password, broker url and its MQTT port.

## Findings on alternative Firmware

See [reflashing](documents/reflashing.md)


## Related links

My [ESP8266 temperature, humidity and CO2 sensor](https://github.com/bakeromso/thermostat_sensor) may be useful as an input for your home automation, for example to help determine when this actuator 