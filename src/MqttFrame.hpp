/*
 * Display frame that shows text passed to it
 *
 * Version: 1.0.1
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

// #define DEBUG_VALUES
// #define DEBUG_MINMAX

#include <string>
#include <OLEDIndexFrame.hpp>
#include "TimeLib.h"
#include <Homie.hpp>

class MqttFrame : public OLEDIndexFrame
{
private:
  const uint8_t cBaseYOffset = 12;
  const uint8_t cSubYOffset = 36;
  float cMaxFloat = 1E100;
  float cMinFloat = -cMaxFloat;
  int _yesterday = -1;
  std::string _name;
  bool _isConfigured = false;
  bool _isOk = false;
  std::vector<std::string> _units;
  std::vector<float> _values;
  std::vector<float> _minValues;
  std::vector<float> _maxValues;
  bool _pageSwitched = true;
  uint8_t _pageIndex = 0;

  void checkPageSwitched(int16_t x);

  void drawAllValues(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y);
  void drawSingleMinMax(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y);
  void drawSingleAndOthers(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y);

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;

  void resetMinMax();

public:
  explicit MqttFrame(const std::string name);

  void loop();
  void onReadyToOperate();

  void clear();
  uint16_t addUnit(const std::string unit);
  uint16_t addValue(const float value);
  bool getIsConfigured();
  void setIsConfigured(const bool value);
  void setIsOk(const bool value);
  void setName(const std::string value);
  void setUnit(const uint8_t index, const std::string value);
  void setValue(const uint8_t index, const float value);
};
