/*
 * An Oled display frame that shows the three day forecast
 * Version: 2.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "ForecastFrame.hpp"

WuForecastFrame::WuForecastFrame(WundergroundClient *wuClient)
    : WuFrame(wuClient) {}

void WuForecastFrame::drawFrame(
    OLEDDisplay &display,
    OLEDDisplayUiState &state,
    int16_t x, int16_t y)
{
  // Skip to the next day at noon
  int baseDay = 0;
  if (isPM())
  {
    baseDay += 2;
  }
  drawForecastDetails(display, x, y, baseDay);
  drawForecastDetails(display, x + 44, y, baseDay + 2);
  drawForecastDetails(display, x + 88, y, baseDay + 4);
}

void WuForecastFrame::drawForecastDetails(
    OLEDDisplay &display,
    int x, int y,
    int dayIndex)
{
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  String day = _wuClient->getForecastTitle(dayIndex).substring(0, 3);
  day.toUpperCase();
  display.drawString(x + 20, y, day);

  display.setFont(Meteocons_Plain_21);
  display.drawString(x + 20, y + 12, _wuClient->getForecastIcon(dayIndex));

  display.setFont(ArialMT_Plain_10);
  display.drawString(x + 20, y + 34, _wuClient->getForecastLowTemp(dayIndex) + "/" + _wuClient->getForecastHighTemp(dayIndex));
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}