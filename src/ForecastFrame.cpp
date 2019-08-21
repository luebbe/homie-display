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
  drawForecastDetails(display, x, y, 0 * FORECASTS_PER_DAY);
  drawForecastDetails(display, x + 44, y, 1 * FORECASTS_PER_DAY);
  drawForecastDetails(display, x + 88, y, 2 * FORECASTS_PER_DAY);
}

void ForecastFrame::drawForecastDetails(
    OLEDDisplay &display,
    int16_t x, int16_t y,
    uint8_t index)
{
  const String DAYS[] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};
  uint8_t dayIndex;
  uint8_t nightIndex;

  if (isPM())
  {
    // First forecast in array is for midnight
    dayIndex = index + 1;
    nightIndex = index;
  }
  else
  {
    // First forecast in array is for noon
    dayIndex = index;
    nightIndex = index + 1;
  }

  time_t observationtime = data[dayIndex].observationTime;
  tm *timeinfo;
  timeinfo = localtime(&observationtime);
  String dayinfo = DAYS[timeinfo->tm_wday];

  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(x + 20, y, dayinfo);

  display.setFont(Meteocons_Plain_21);
  display.drawString(x + 20, y + 12, data[dayIndex].iconMeteoCon);

  display.setFont(ArialMT_Plain_10);
  display.drawString(x + 20, y + 34, String(data[nightIndex].tempMin) + "/" + String(data[dayIndex].tempMax));
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void ForecastFrame::update(String apiKey, String locationId, String language, boolean isMetric)
{
  client.setLanguage(language);
  client.setMetric(isMetric);

  // Forecasts to fetch for each day
  uint8_t HOURS_TO_FETCH[FORECASTS_PER_DAY] = {0, 12};

  client.setAllowedHours(HOURS_TO_FETCH, FORECASTS_PER_DAY);
  client.updateForecastsById(data, apiKey, locationId, MAX_FORECASTS);
}
