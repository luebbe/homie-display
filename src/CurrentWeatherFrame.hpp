/*
 * An Oled display frame that shows the current weather
 * form openweathermap.org
 * Version: 3.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <OLEDIndexFrame.hpp>
#include "OpenWeatherMapCurrent.h"
#include "WeatherStationFonts.h"

class CurrentWeatherFrame : public OLEDIndexFrame
{
private:
  OpenWeatherMapCurrent client;
  OpenWeatherMapCurrentData data;

public:
  explicit CurrentWeatherFrame();

  void update(String apiKey, String locationId, String language, boolean isMetric);

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;
};
