# Homie Display

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Several Homie node(s) that wrap different data sources and show them on an OLED display

The software is based on [Homie (v2.0)](https://github.com/marvinroger/homie-esp8266) and is developed using [PlatformIO](https://github.com/platformio)

## Dependencies
* Homie 2.0 develop is installed in the global library storage
* The NTP Client lib (PlatformIO 551) is installed in the global library storage

## Weather Underground
It uses files copied from [ESP8266 Weather Station](https://github.com/squix78/esp8266-weather-station) to access the weather underground API. These files are copied into the lib subdirectory. The entire weather station library is not included, because it has its own NTP client with a different interface than the one that I prefer.

## Case
A friend of mine printed this cute little case for me: [Desktop Widget](https://www.thingiverse.com/thing:857858/#files)

<img src="./images/Case_Front1.jpg" height="200">
Front view showing current weather


<img src="./images/Case_Front2.jpg" height="200">
Front view showing three day forecast


<img src="./images/Case_Rear.jpg" height="200">
Rear view
