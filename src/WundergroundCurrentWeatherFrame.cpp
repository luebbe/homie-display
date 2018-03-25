/*
 * An Oled display frame that shows the current weather
 * from weather underground
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "WundergroundCurrentWeatherFrame.hpp"

WuCurrentWeatherFrame::WuCurrentWeatherFrame(WundergroundClient *wuClient)
    : WuFrame(wuClient) {}

void WuCurrentWeatherFrame::drawFrame(
    OLEDDisplay &display,
    OLEDDisplayUiState &state,
    int16_t x, int16_t y)
{
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(58 + x, 5 + y, _wuClient->getWeatherText());

  display.setFont(ArialMT_Plain_24);
  String temp = _wuClient->getCurrentTemp() + "°C";
  display.drawString(58 + x, 15 + y, temp);
  int tempWidth = display.getStringWidth(temp);

  display.setFont(Meteocons_Plain_42);
  String weatherIcon = _wuClient->getTodayIcon();
  int weatherIconWidth = display.getStringWidth(weatherIcon);
  display.drawString(30 + x - weatherIconWidth / 2, 05 + y, weatherIcon);
}
