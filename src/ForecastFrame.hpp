/*
 * An Oled display frame that shows the three day forecast
 * form openweathermap.org
 * Version: 3.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <OLEDIndexFrame.hpp>
#include "TimeLib.h"
#include "WeatherStationFonts.h"

class ForecastFrame : public OLEDIndexFrame
{
private:
  void drawForecastDetails(OLEDDisplay &display, int x, int y, int dayIndex);

public:
  explicit ForecastFrame();

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;
};
