/*
 * Homie node that fetches data from openweathermap.org
 * and shows it on an Oled display
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include <Homie.hpp>
#include "OwmNode.hpp"

// OpenWeatherMap Settings
HomieSetting<const char *> owmApiKey("OwmApiKey", "Your open weather map API key");
HomieSetting<const char *> owmLanguage("OwmLanguage", "The language in which you want to retrieve the weather information");
HomieSetting<const char *> owmLocationId("OwmLocationId", "The location for which you want to retrieve the weather information");
HomieSetting<long> owmUpdate("OwmUpdate", "The update interval in minutes for openweathermap.org (must be at least 10 minutes)");

OwmNode::OwmNode(const char *name)
    : HomieNode(name, "OpenWeatherMap")
{
  _owmCurrent = new CurrentWeatherFrame();
  _owmForecast = new ForecastFrame();
  _nextUpdate = 0;
}

void OwmNode::beforeSetup()
{
  // This has to be called before Homie.setup, because otherwise the default Values will
  // override the values which were already read from config.json
  Homie.getLogger() << "• OwmNode - Before setup" << endl;
  owmApiKey.setDefaultValue(OWM_API_KEY);
  owmLocationId.setDefaultValue(OWM_LOCATIONID);
  owmLanguage.setDefaultValue(OWM_LANGUAGE);
  owmUpdate.setDefaultValue(OWM_UPDATE).setValidator([](long candidate) {
    return (candidate >= 10) && (candidate <= 24 * 6 * 10); // Update interval etween 10 minutes and 24 hours
  });
}

void OwmNode::onReadyToOperate()
{
  Homie.getLogger() << "• OwmNode - onReadyToOperate" << endl;
  _nextUpdate = millis() + 5000; // Wait 5 seconds before fetching data form openweathermap.org for the first time
};

bool OwmNode::isConfigured()
{
  return owmApiKey.wasProvided() && owmLocationId.wasProvided();
}

void OwmNode::loop()
{
  if (isConfigured())
  {
    if (millis() >= _nextUpdate)
    {
      _owmCurrent->update(owmApiKey.get(), owmLocationId.get(), owmLanguage.get(), IS_METRIC);
      _owmForecast->update(owmApiKey.get(), owmLocationId.get(), owmLanguage.get(), IS_METRIC);
      _nextUpdate = millis() + owmUpdate.get() * 60000UL;
    }
  }
};
