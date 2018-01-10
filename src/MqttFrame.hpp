/*
 * Display frame that shows text passed to it
 *
 * Version: 1.0.1
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <string>
#include <OLEDIndexFrame.hpp>

class MqttFrame : public OLEDIndexFrame
{
private:
  std::string _name;
  bool _isConfigured = false;
  bool _isOk = false;
  std::vector<std::string> _values;
  std::vector<std::string> _units;

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;

public:
  MqttFrame(const std::string name);

  void clear();
  unsigned int addUnit(const std::string unit);
  unsigned int addValue(const std::string value);
  bool getIsConfigured();
  void setIsConfigured(const bool value);
  void setIsOk(const bool value);
  void setName(std::string value);
  void setUnit(int index, const std::string unit);
  void setValue(int index, const std::string value);
};
