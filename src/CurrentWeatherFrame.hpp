/*
 * An Oled display frame that shows the current weather
 *
 * Version: 2.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <WeatherFrame.hpp>

class WuCurrentWeatherFrame : public WuFrame
{
public:
  explicit WuCurrentWeatherFrame(WundergroundClient *wuClient);

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;
};
