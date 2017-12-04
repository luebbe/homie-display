# Homie Display

[![Travis](https://img.shields.io/travis/luebbe/homie-display.svg?branch=master&style=flat)](https://travis-ci.org/luebbe/homie-display)
[![License](https://img.shields.io/github/license/mashape/apistatus.svg?style=flat)](https://opensource.org/licenses/MIT)
[![GitHub release](https://img.shields.io/github/release/luebbe/homie-display.svg?style=flat)](https://github.com/luebbe/homie-display/releases)

Several Homie node(s) that wrap different data sources and show them on an OLED display

The software is based on [Homie (v2.0)](https://github.com/marvinroger/homie-esp8266) and is developed using [PlatformIO](https://github.com/platformio)

## Dependencies
All dependencies are included via platformio.ini

## Weather Underground
It uses files copied from [ESP8266 Weather Station](https://github.com/squix78/esp8266-weather-station) to access the weather underground API. These files are copied into the lib subdirectory. The entire weather station library is not included, because it has its own NTP client with a different interface than the one that I prefer.

## Case
A friend of mine printed this cute little case for me: [Desktop Widget](https://www.thingiverse.com/thing:857858/#files)

| <img src="./images/Case_Front1.jpg" align="center" width="200"> | <img src="./images/Case_Front2.jpg" align="center" width="200"> | <img src="./images/Case_Rear.jpg" align="center" width="200"> |
|:---:|:---:|:---:|
|  Front view showing current weather | Front view showing three day forecast | Rear view |
