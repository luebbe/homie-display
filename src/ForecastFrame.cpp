/*
 * An Oled display frame that shows the three day forecast
 * form openweathermap.org
 * Version: 3.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "ForecastFrame.hpp"

ForecastFrame::ForecastFrame()
{
}

void ForecastFrame::drawFrame(
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

void ForecastFrame::drawForecastDetails(
    OLEDDisplay &display,
    int x, int y,
    int dayIndex)
{
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  // String day = data[dayIndex]
  //   //_wuClient->getForecastTitle(dayIndex).substring(0, 3);
  // day.toUpperCase();
  // display.drawString(x + 20, y, day);

  display.setFont(Meteocons_Plain_21);
  display.drawString(x + 20, y + 12, data[dayIndex].iconMeteoCon);

  display.setFont(ArialMT_Plain_10);
  display.drawString(x + 20, y + 34, String(data[dayIndex].tempMin, 1) + "/" + String(data[dayIndex].tempMax, 1));
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void ForecastFrame::update(String apiKey, String locationId, String language, boolean isMetric)
{
  client.setLanguage(language);
  client.setMetric(isMetric);

  uint8_t allowedHours[] = {0, 12};
  client.setAllowedHours(allowedHours, 2);
  uint8_t foundForecasts = client.updateForecastsById(data, apiKey, locationId, MAX_FORECASTS);
}
