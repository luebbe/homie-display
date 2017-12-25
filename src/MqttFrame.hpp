/*
 * Display frame that shows text passed to it
 *
 * Version: 1.0.1
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <OLEDIndexFrame.hpp>

class MqttFrame : public OLEDIndexFrame
{
private:
  String _name;
  std::vector<String> _values;
  std::vector<String> _units;

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;

public:
  MqttFrame(const String name);

  unsigned int addValue(const String value, const String unit);
  void setName(const String value);
  void setUnit(int index, const String unit);
  void setValue(int index, const String value);
};
