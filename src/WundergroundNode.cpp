/*
 * Homie node that fetches data from weather underground
 * and shows it on an Oled display
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include <Homie.hpp>
#include "WundergroundNode.hpp"

// Wunderground Settings
HomieSetting<const char *> wundergroundApiKey("WuApiKey", "Your weather underground API key");
HomieSetting<const char *> wundergroundLanguage("WuLanguage", "The language in which you want to retrieve the weather underground data");
HomieSetting<const char *> wundergroundCountry("WuCountry", "The country in which your citie lies");
HomieSetting<const char *> wundergroundCity("WuCity", "The city for which you want to retrieve the weather underground data");
HomieSetting<long> wundergroundUpdate("WuUpdate", "The update interval in minutes for weather underground (must be at least 10 minutes)");

WundergroundNode::WundergroundNode(const char *name)
    : HomieNode(name, "Wunderground")
{
  _wuClient = new WundergroundClient(IS_METRIC);
  _wuCurrent = new WuCurrentWeatherFrame(_wuClient);
  _wuForecast = new WuForecastFrame(_wuClient);
  _nextUpdate = 0;
}

void WundergroundNode::beforeSetup()
{
  // This has to be called before Homie.setup, because otherwise the default Values will
  // override the values which were already read from config.json
  Homie.getLogger() << "• WundergroundNode - Before setup" << endl;
  wundergroundApiKey.setDefaultValue(WUNDERGRROUND_API_KEY);
  wundergroundLanguage.setDefaultValue(WUNDERGRROUND_LANGUAGE);
  wundergroundCountry.setDefaultValue(WUNDERGROUND_COUNTRY);
  wundergroundCity.setDefaultValue(WUNDERGROUND_CITY);
  wundergroundUpdate.setDefaultValue(WUNDERGROUND_UPDATE).setValidator([](long candidate) {
    return (candidate >= 10) && (candidate <= 24 * 6 * 10); // Update interval etween 10 minutes and 24 hours
  });
}

void WundergroundNode::setupHandler()
{
  Homie.getLogger() << "• WundergroundNode - Setuphandler" << endl;
  _nextUpdate = millis() + 5000; // Wait 5 seconds before fetching weather underground data for the first time
};

bool WundergroundNode::isConfigured()
{
  return wundergroundApiKey.wasProvided() && wundergroundCity.wasProvided();
}

void WundergroundNode::loop()
{
  if (isConfigured())
  {
    if (millis() >= _nextUpdate)
    {
      _wuClient->updateConditions(wundergroundApiKey.get(), wundergroundLanguage.get(), wundergroundCountry.get(), wundergroundCity.get());
      _wuClient->updateForecast(wundergroundApiKey.get(), wundergroundLanguage.get(), wundergroundCountry.get(), wundergroundCity.get());
      _nextUpdate = millis() + wundergroundUpdate.get() * 60000UL;
    }
  }
};
