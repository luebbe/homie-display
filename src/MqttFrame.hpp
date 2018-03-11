/*
 * Display frame that shows text passed to it
 *
 * Version: 1.0.1
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <string>
#include <OLEDIndexFrame.hpp>
#ifdef DEBUGMINMAX
#include <Homie.hpp>
#endif

class MqttFrame : public OLEDIndexFrame
{
private:
  float cMaxFloat = 1E100;
  float cMinFloat = -cMaxFloat;
  std::string _name;
  bool _isConfigured = false;
  bool _isOk = false;
  std::vector<std::string> _units;
  std::vector<float> _values;
  std::vector<float> _minValues;
  std::vector<float> _maxValues;  
  bool _curPageCounted = false;
  uint8_t _curPage = 0;

  void drawAllValues(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y);
  void drawSingleValue(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y);

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;

public:
  MqttFrame(const std::string name);

  void clear();
  unsigned int addUnit(const std::string unit);
  unsigned int addValue(const float value);
  void resetMinMax();
  bool getIsConfigured();
  void setIsConfigured(const bool value);
  void setIsOk(const bool value);
  void setName(std::string value);
  void setUnit(int index, const std::string unit);
  void setValue(int index, const float value);
};
