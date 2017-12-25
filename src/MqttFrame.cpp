/*
 * Display frame that shows text passed to it
 *
 * Version: 1.0.1
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "MqttFrame.hpp"

MqttFrame::MqttFrame(const String name)
{
  _name = name;
}

unsigned int MqttFrame::addValue(const String value, const String unit)
{
  _values.push_back(value);
  _units.push_back(unit);
  return _values.size();
}

void MqttFrame::setName(const String name)
{
  _name = name;
};

void MqttFrame::setUnit(int index, const String unit)
{
  if (index <= _units.size())
  {
    _units[index] = unit;
  }
}

void MqttFrame::setValue(int index, const String value)
{
  if (index <= _values.size())
  {
    _values[index] = value;
  }
}

// Interface OLEDFrame
void MqttFrame::drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y)
{
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128 + x, y, _name);

  // Align output at space between value and unit
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  for (int i = 0; i < _values.size(); i++)
  {
    display.drawString(x + 78, y + 11 + 20 * i, _values[i]);
  }
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  for (int i = 0; i < _units.size(); i++)
  {
    display.drawString(x + 80, y + 11 + 20 * i, _units[i]);
  }
};
