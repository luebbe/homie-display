/*
 * An Oled display frame that shows the three day forecast
 * form openweathermap.org
 * Version: 3.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <OLEDIndexFrame.hpp>
#include "OpenWeatherMapForecast.h"
#include "WeatherStationFonts.h"

class ForecastFrame : public OLEDIndexFrame
{
private:
  static const uint8_t MAX_FORECASTS = 15;
  OpenWeatherMapForecast client;
  OpenWeatherMapForecastData data[MAX_FORECASTS];

  void drawForecastDetails(OLEDDisplay &display, int x, int y, int dayIndex);

public:
  explicit ForecastFrame();

  void update(String apiKey, String locationId, String language, boolean isMetric);

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;
};
