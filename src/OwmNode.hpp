/*
 * Homie node that fetches data from openweathermap.org
 * and shows it on an Oled display
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <HomieNode.hpp>
#include "CurrentWeatherFrame.hpp"
#include "ForecastFrame.hpp"

class OwmNode : public HomieNode
{
private:
  const boolean IS_METRIC = true;
  const char *OWM_API_KEY = "YOUR_API_KEY";
  const char *OWM_LANGUAGE = "EN";
  const char *OWM_LOCATIONID = "YOURLOCATIONID";
  const long OWM_UPDATE = 15; // Default update every 15 minutes

  CurrentWeatherFrame *_owmCurrent;
  ForecastFrame *_owmForecast;
  unsigned long _nextUpdate;
  bool isConfigured();

protected:
  virtual void loop() override;
  virtual void onReadyToOperate() override;

public:
  explicit OwmNode(const char *name);
  void beforeSetup();
};
