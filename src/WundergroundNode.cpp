/*
 * Homie node that fetches data from weather underground
 * and shows it on an Oled display
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include <Homie.hpp>
#include "WundergroundNode.hpp"

// Wunderground Settings
const boolean IS_METRIC = true;
const String WUNDERGRROUND_API_KEY = "YOUR_API_KEY";
const String WUNDERGRROUND_LANGUAGE = "DE";
const String WUNDERGROUND_COUNTRY = "DE";
const String WUNDERGROUND_CITY = "YOURTOWN";

WundergroundNode::WundergroundNode(const char *name)
: HomieNode(name, "Wunderground") {
  _wuClient = new WundergroundClient(IS_METRIC);
  _wuCurrent = new WuCurrentWeatherFrame(_wuClient);
	_wuForecast = new WuForecastFrame(_wuClient);
  _updateInterval = UPDATE_INTERVAL;
  _lastUpdate = 0;
}

void WundergroundNode::setup() {
  Homie.getLogger() << "WundergroundNode Setup" << endl;
};

void WundergroundNode::loop() {
  if (millis() - _lastUpdate >= _updateInterval * 1000UL || _lastUpdate == 0) {
    _wuClient->updateConditions(WUNDERGRROUND_API_KEY, WUNDERGRROUND_LANGUAGE, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY);
    _wuClient->updateForecast(WUNDERGRROUND_API_KEY, WUNDERGRROUND_LANGUAGE, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY);
    _lastUpdate = millis();
  }
};
