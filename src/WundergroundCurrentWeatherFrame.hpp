/*
 * An Oled display frame that shows the current weather
 * from weather underground
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <WundergroundFrame.hpp>

class WuCurrentWeatherFrame : public WuFrame
{
public:
  WuCurrentWeatherFrame(WundergroundClient *wuClient, NTPClient timeClient);

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;
};
