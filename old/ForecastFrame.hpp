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
#include "TimeLib.h"

class ForecastFrame : public OLEDIndexFrame
{
private:
  static const uint8_t FORECAST_DAYS = 4;
  static const uint8_t FORECASTS_PER_DAY = 2;
  static const uint8_t MAX_FORECASTS = FORECAST_DAYS * FORECASTS_PER_DAY;

  OpenWeatherMapForecast client;
  OpenWeatherMapForecastData data[MAX_FORECASTS];

  void drawForecastDetails(OLEDDisplay &display, int16_t x, int16_t y, uint8_t index);

public:
  explicit ForecastFrame();

  void update(String apiKey, String locationId, String language, boolean isMetric);

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;
};
