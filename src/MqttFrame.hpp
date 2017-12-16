/*
 * Display frame that shows text passed to it
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <OLEDIndexFrame.hpp>

class MqttFrame : public OLEDIndexFrame
{
private:
  String _name;
  String _temp = "ttt";
  String _humid = "hhh";

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;

public:
  MqttFrame(const char *name);

  void setName(String const value)
  {
    _name = value;
  };
  void setHumidity(String const value)
  {
    _humid = value;
  };
  void setTemperature(String const value)
  {
    _temp = value;
  };
};
