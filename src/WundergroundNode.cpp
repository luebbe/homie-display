/*
 * Homie node that fetches data from weather underground
 * and shows it on an Oled display
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include <Homie.hpp>
#include "WundergroundNode.hpp"

HomieSetting<const char*> wundergroundApiKey("WuApiKey", "Your weather underground API key");
HomieSetting<const char*> wundergroundLanguage("WuLanguage", "The language in which you want to retrieve the weather underground data");
HomieSetting<const char*> wundergroundCountry("WuCountry", "The country in which your citie lies");
HomieSetting<const char*> wundergroundCity("WuCity", "The city for which you want to retrieve the weather underground data");

// Wunderground Settings
const boolean IS_METRIC = true;
const char * WUNDERGRROUND_API_KEY = "YOUR_API_KEY";
const char * WUNDERGRROUND_LANGUAGE = "DE";
const char * WUNDERGROUND_COUNTRY = "DE";
const char * WUNDERGROUND_CITY = "YOURTOWN";

WundergroundNode::WundergroundNode(const char *name)
: HomieNode(name, "Wunderground") {
  _wuClient = new WundergroundClient(IS_METRIC);
  _wuCurrent = new WuCurrentWeatherFrame(_wuClient);
  _wuForecast = new WuForecastFrame(_wuClient);
  _updateInterval = UPDATE_INTERVAL;
  _lastUpdate = 0;
  wundergroundApiKey.setDefaultValue(WUNDERGRROUND_API_KEY);
  wundergroundLanguage.setDefaultValue(WUNDERGRROUND_LANGUAGE);
  wundergroundCountry.setDefaultValue(WUNDERGROUND_COUNTRY);
  wundergroundCity.setDefaultValue(WUNDERGROUND_CITY);
}

void WundergroundNode::setup() {
  Homie.getLogger() << "WundergroundNode Setup" << endl;
};

bool WundergroundNode::isConfigured() {
  return wundergroundApiKey.wasProvided() && wundergroundCity.wasProvided();
}

void WundergroundNode::loop() {
  if (isConfigured()) {
    if (millis() - _lastUpdate >= _updateInterval * 1000UL || _lastUpdate == 0) {
      _wuClient->updateConditions(wundergroundApiKey.get(), wundergroundLanguage.get(), wundergroundCountry.get(), wundergroundCity.get());
      _wuClient->updateForecast(wundergroundApiKey.get(), wundergroundLanguage.get(), wundergroundCountry.get(), wundergroundCity.get());
      _lastUpdate = millis();
    }
  }
};
