/*
 * An Oled display frame that shows the three day forecast
 * Version: 2.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <WeatherFrame.hpp>

class WuForecastFrame : public WuFrame
{
private:
  void drawForecastDetails(OLEDDisplay &display, int x, int y, int dayIndex);

public:
  explicit WuForecastFrame(WundergroundClient *wuClient);

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;
};
