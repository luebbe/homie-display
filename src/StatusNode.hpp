/*
 * A Homie node that shows (Homie) status information
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <OLEDIndexFrame.hpp>
#include "TimeClientHelper.hpp"
#include "images.h"

class StatusNode : public HomieNode, public OLEDIndexFrame
{
private:
  const char *_name;
  const char *_fw_name;
  const char *_fw_version;
  bool _cfgmode;
  bool _mqtt;
  bool _wifi;
  bool _alert;
  long _milliseconds;
  long _lasttick;
  String _statusText;
  String _alertMessage;

  // Interface HomieNode
  bool handleBroadcast(const String &level, const String &value);

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y) override;

  // Interface OLEDStatusIndicator
  virtual void drawOverlay(OLEDDisplay &display, OLEDDisplayUiState &state, uint8_t idx);

  void drawWifiStrength(OLEDDisplay &display);

protected:
  virtual void loop() override;

public:
  StatusNode(const char *name, const char *fw_name, const char *fw_version);

  void event(const HomieEvent &event);

  void setStatusText(String const value)
  {
    _statusText = value;
  };
  bool isAlert()
  {
    return _alert;
  }
  bool isWifiConnected()
  {
    return _wifi;
  };
};
