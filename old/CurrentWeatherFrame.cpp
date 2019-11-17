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
  // Icon font width = 42 pixels
  display.setFont(Meteocons_Plain_42);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(24 + x, 5 + y, data.iconMeteoCon);

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(48 + x, 5 + y, data.description);

  display.setFont(ArialMT_Plain_24);
  String temp = String(data.temp, 1) + "°C";
  display.drawString(48 + x, 15 + y, temp);
}

void CurrentWeatherFrame::update(String apiKey, String locationId, String language, boolean isMetric)
{
  client.setLanguage(language);
  client.setMetric(isMetric);
  client.updateCurrentById(&data, apiKey, locationId);
}
