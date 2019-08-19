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
  drawForecastDetails(display, x, y, 0);
  drawForecastDetails(display, x + 44, y, 1);
  drawForecastDetails(display, x + 88, y, 2);
}

void ForecastFrame::drawForecastDetails(
    OLEDDisplay &display,
    int x, int y,
    int dayIndex)
{
  // time_t time;
  // time = data[dayIndex].observationTime;
  String day = "";
  // char *day = ctime(&time); //.substring(0, 3);
  // strftime(day, 3, "a", &time); //.substring(0, 3);

  // String day = data[dayIndex].
  //   //_wuClient->getForecastTitle(dayIndex).substring(0, 3);
  // day.toUpperCase();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(x + 20, y, day);

  display.setFont(Meteocons_Plain_21);
  display.drawString(x + 20, y + 12, data[dayIndex].iconMeteoCon);

  display.setFont(ArialMT_Plain_10);
  display.drawString(x + 20, y + 34, String(data[dayIndex].tempMin, 0) + "/" + String(data[dayIndex].tempMax, 0));
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void ForecastFrame::update(String apiKey, String locationId, String language, boolean isMetric)
{
  client.setLanguage(language);
  client.setMetric(isMetric);

  // Fetch alternating forecasts for night/day
  uint8_t allowedHours[] = {0, 12};
  client.setAllowedHours(allowedHours, 2);
  client.updateForecastsById(data, apiKey, locationId, MAX_FORECASTS);
}
