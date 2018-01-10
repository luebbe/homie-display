/*
 * Display frame that shows text passed to it
 *
 * Version: 1.0.1
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "MqttFrame.hpp"

MqttFrame::MqttFrame(const std::string name)
{
  _name = name;
  _isConfigured = false;
}

void MqttFrame::clear()
{
  _units.clear();
  _values.clear();
  _isConfigured = false;
}

unsigned int MqttFrame::addUnit(const std::string unit)
{
  _units.push_back(unit);
  return _units.size();
}

unsigned int MqttFrame::addValue(const std::string value)
{
  _values.push_back(value);
  return _values.size();
}

bool MqttFrame::getIsConfigured()
{
  return _isConfigured;
}

void MqttFrame::setIsConfigured(const bool value)
{
  _isConfigured = value;
}

void MqttFrame::setIsOk(const bool value)
{
  _isOk = value;
}

void MqttFrame::setName(const std::string name)
{
  _name = name;
};

void MqttFrame::setUnit(int index, const std::string unit)
{
  if (index <= _units.size())
  {
    _units[index] = unit;
  }
}

void MqttFrame::setValue(int index, const std::string value)
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
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(x, y, _name.c_str());

  if (_isOk)
  {
    // Select different font size depending on the number of values to display
    // works fine with 0..3 parameters
    int baseoffset = 12;
    int rowoffset = 0;
    if (_values.size() <= 1)
    {
      display.setFont(ArialMT_Plain_24);
      rowoffset = 30;
    }
    else if (_values.size() <= 2)
    {
      display.setFont(ArialMT_Plain_16);
      rowoffset = 20;
    }
    else
    {
      display.setFont(ArialMT_Plain_10);
      rowoffset = 12;
    }

    // Align output at space between value and unit
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    for (int i = 0; i < _values.size(); i++)
    {
      display.drawString(x + 78, y + baseoffset + rowoffset * i, _values[i].c_str());
    }
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    for (int i = 0; i < _units.size(); i++)
    {
      display.drawString(x + 80, y + baseoffset + rowoffset * i, _units[i].c_str());
    }
  }
  else
  {
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(x + 64, y + 12, "Error");
  }
};