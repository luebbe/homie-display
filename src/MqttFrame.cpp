/*
 * Display frame that shows text passed to it
 * 
 * Two alternative modes
 * - drawAllValues shows all values on the same screen
 * - drawSingleMinMax cycles through the values and shows only one at a time plus its min/max values below
 * - drawSingleAndOthers cycles through the values and shows only one at a time plus two other values below
 *
 * Version: 1.0.1
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "MqttFrame.hpp"

MqttFrame::MqttFrame(const std::string name)
    : _name(name),
      _isConfigured(false)
{
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

void MqttFrame::onReadyToOperate()
{
  _yesterday = day();
}

uint16_t MqttFrame::addUnit(const std::string unit)
{
  _units.push_back(unit);
#ifdef DEBUG_VALUES
  Homie.getLogger() << "Add unit " << unit.c_str() << " " << _units.size() << endl;
#endif
  return _units.size();
}

uint16_t MqttFrame::addValue(const float value)
{
  _values.push_back(value);
  _minValues.push_back(cMaxFloat);
  _maxValues.push_back(cMinFloat);
#ifdef DEBUG_VALUES
  Homie.getLogger() << "Add value " << String(value) << " " << _values.size() << endl;
#endif
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

void MqttFrame::setName(const std::string value)
{
  _name = value;
}

void MqttFrame::setUnit(const uint8_t index, const std::string value)
{
  if (index < _units.size())
  {
    _units[index] = value;
  }
}

void MqttFrame::setValue(const uint8_t index, const float value)
{
  if (index < _values.size())
  {
    _values[index] = value;
    if (value < _minValues[index])
    {
#ifdef DEBUG_MINMAX
      Homie.getLogger() << "Min[" << index << "] " << _minValues[index] << "->" << value << endl;
#endif
      _minValues[index] = value;
    }
    if (value > _maxValues[index])
    {
#ifdef DEBUG_MINMAX
      Homie.getLogger() << "Max[" << index << "] " << _maxValues[index] << "->" << value << endl;
#endif
      _maxValues[index] = value;
    }
  }
}

void MqttFrame::resetMinMax()
{
  for (uint8_t i = 0; i < _minValues.size(); i++)
  {
#ifdef DEBUG_MINMAX
    Homie.getLogger() << "Min/Max[" << i << "] " << _minValues[i] << "/" << _maxValues[i] << "->" << _values[i] << endl;
#endif
    _minValues[i] = _values[i];
    _maxValues[i] = _values[i];
  }
}

void MqttFrame::checkPageSwitched(int16_t x)
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
}

void MqttFrame::drawAllValues(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y)
{
  // Draws all values at the same time
  // Select different font size depending on the number of values to display
  // works fine with 0..3 parameters
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
  char *outVal = NULL;
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  for (uint8_t i = 0; i < _values.size(); i++)
  {
    asprintf(&outVal, "%3.1f", _values[i]);
    display.drawString(x + 78, y + cBaseYOffset + rowoffset * i, outVal);
    free(outVal);
  }
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  for (uint8_t i = 0; i < _units.size(); i++)
  {
    display.drawString(x + 80, y + cBaseYOffset + rowoffset * i, _units[i].c_str());
  }
}

void MqttFrame::drawSingleMinMax(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y)
{
  // Cycles through the collected values and draws a single value (modulo number of values)
  // Displays its min/max values below it

  checkPageSwitched(x);

  char *outVal = NULL;

  display.setTextAlignment(TEXT_ALIGN_CENTER);

  display.setFont(ArialMT_Plain_24);
  asprintf(&outVal, "%3.1f%s", _values[_pageIndex], _units[_pageIndex].c_str());
  display.drawString(x + DISPLAY_WIDTH / 2, y + cBaseYOffset, outVal);
  free(outVal);

  display.setFont(ArialMT_Plain_10);
  asprintf(&outVal, "%3.1f/%3.1f%s", _minValues[_pageIndex], _maxValues[_pageIndex], _units[_pageIndex].c_str());
  display.drawString(x + DISPLAY_WIDTH / 2, y + cSubYOffset, outVal);
  free(outVal);
}

void MqttFrame::drawSingleAndOthers(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y)
{
  // Cycles through the collected values and draws a single value (modulo number of values)
  // Displays upt to two other values in smaller print below it

  checkPageSwitched(x);

  char *outVal = NULL;

  display.setTextAlignment(TEXT_ALIGN_CENTER);

  display.setFont(ArialMT_Plain_24);
  asprintf(&outVal, "%3.1f%s", _values[_pageIndex], _units[_pageIndex].c_str());
  display.drawString(x + DISPLAY_WIDTH / 2, y + cBaseYOffset, outVal);
  free(outVal);

  uint8_t pred = (_pageIndex - 1) % _values.size();
  uint8_t succ = (_pageIndex + 1) % _values.size();

  if (pred == succ)
    asprintf(&outVal, "%3.1f%s", _values[pred], _units[pred].c_str());
  else
    asprintf(&outVal, "%3.1f%s/%3.1f%s", _values[pred], _units[pred].c_str(), _values[succ], _units[succ].c_str());

  display.setFont(ArialMT_Plain_10);
  display.drawString(x + DISPLAY_WIDTH / 2, y + cSubYOffset, outVal);
  free(outVal);
}

// Interface OLEDFrame
void MqttFrame::drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y)
{
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(x, y, _name.c_str());

  if (_isOk)
  {
    // Maybe make this choice configurable as a homie setting in the future
    // drawAllValues(display, state, x, y);
    // drawSingleMinMax(display, state, x, y);
    drawSingleAndOthers(display, state, x, y);
  }
  else
  {
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(x + DISPLAY_WIDTH / 2, y + 12, "Error");
  }
}
