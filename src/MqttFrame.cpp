/*
 * Display frame that shows text passed to it
 * 
 * Two alternative modes
 * - drawAllValues shows all values on the same screen
 * - drawSingleValue cycels through the values and shows only one at a time
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
  _minValues.clear();
  _maxValues.clear();
  _isConfigured = false;
}

void MqttFrame::loop()
{
  // Reset min/max values at midnight
  if (day() > _yesterday)
  {
    _yesterday = day();
    resetMinMax();
  }
}

void MqttFrame::setupHandler()
{
  _yesterday = day();
}

unsigned int MqttFrame::addUnit(const std::string unit)
{
  _units.push_back(unit);
  return _units.size();
}

unsigned int MqttFrame::addValue(const float value)
{
  _values.push_back(value);
  _minValues.push_back(cMaxFloat);
  _maxValues.push_back(cMinFloat);
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
  if (index < _units.size())
  {
    _units[index] = unit;
  }
}

void MqttFrame::setValue(int index, const float value)
{
  if (index < _values.size())
  {
    _values[index] = value;
    if (value < _minValues[index])
    {
#ifdef DEBUGMINMAX
      Homie.getLogger() << "Min[" << index << "] " << _minValues[index] << "->" << value << endl;
#endif
      _minValues[index] = value;
    }
    if (value > _maxValues[index])
    {
#ifdef DEBUGMINMAX
      Homie.getLogger() << "Max[" << index << "] " << _maxValues[index] << "->" << value << endl;
#endif
      _maxValues[index] = value;
    }
  }
}

void MqttFrame::resetMinMax()
{
  for (int i = 0; i <= _minValues.size(); i++)
  {
#ifdef DEBUGMINMAX
    Homie.getLogger() << "Min/Max[" << i << "] " << _minValues[i] << "/" << _maxValues[i] << "->" << _values[i] << endl;
#endif
    _minValues[i] = _values[i];
    _maxValues[i] = _values[i];
  }
}

void MqttFrame::drawAllValues(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y)
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
  char tempString[20] = "";
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  for (int i = 0; i < _values.size(); i++)
  {
    sprintf(tempString, "%3.1f", _values[i]);
    display.drawString(x + 78, y + baseoffset + rowoffset * i, tempString);
  }
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  for (int i = 0; i < _units.size(); i++)
  {
    display.drawString(x + 80, y + baseoffset + rowoffset * i, _units[i].c_str());
  }
}

void MqttFrame::drawSingleValue(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y)
{
  if (x > 0)
  {
    if (!_pageSwitched)
    {
      _pageIndex = (_pageIndex + 1) % _values.size();
      _pageSwitched = true;
    }
  }
  else
  {
    _pageSwitched = false;
  }

  int baseoffset = 12;
  display.setFont(ArialMT_Plain_24);

  char tempString[20] = "";
  sprintf(tempString, "%3.1f", _values[_pageIndex]);

  // Align output at space between value and unit
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(x + 84, y + baseoffset, tempString);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(x + 86, y + baseoffset, _units[_pageIndex].c_str());

  sprintf(tempString, "%3.1f/%3.1f%s", _minValues[_pageIndex], _maxValues[_pageIndex], _units[_pageIndex].c_str());

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(x + 64, y + 36, tempString);
}

// Interface OLEDFrame
void MqttFrame::drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y)
{
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(x, y, _name.c_str());

  if (_isOk)
  {
    // drawAllValues(display, state, x, y);
    drawSingleValue(display, state, x, y);
  }
  else
  {
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(x + 64, y + 12, "Error");
  }
};
