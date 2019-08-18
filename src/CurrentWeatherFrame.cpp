/*
 * An Oled display frame that shows the current weather
 * form openweathermap.org
 * Version: 3.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "CurrentWeatherFrame.hpp"

CurrentWeatherFrame::CurrentWeatherFrame()
{
}

void CurrentWeatherFrame::drawFrame(
    OLEDDisplay &display,
    OLEDDisplayUiState &state,
    int16_t x, int16_t y)
{
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(58 + x, 5 + y, data.description);

  display.setFont(ArialMT_Plain_24);
  String temp = String(data.temp, 1) + "°C";
  display.drawString(58 + x, 15 + y, temp);

  display.setFont(Meteocons_Plain_42);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  String weatherIcon = data.iconMeteoCon;
  int weatherIconWidth = display.getStringWidth(weatherIcon);
  display.drawString(30 + x - weatherIconWidth / 2, 5 + y, weatherIcon);
}

void CurrentWeatherFrame::update(String apiKey, String locationId, String language, boolean isMetric)
{
  client.setLanguage(language);
  client.setMetric(isMetric);
  client.updateCurrentById(&data, apiKey, locationId);
}
