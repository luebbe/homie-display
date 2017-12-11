/*
 * A Homie node that shows (Homie) status information
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#ifndef SRC_STATUSNODE_H_
#define SRC_STATUSNODE_H_

#include <OLEDIndexFrame.hpp>
#include "images.h"

class StatusNode: public HomieNode, public OLEDIndexFrame {
private:
  const char *_name;
  bool _cfgmode;
  bool _mqtt;
  bool _wifi;
  bool _alert;
  long _milliseconds;
  long _lasttick;
  String _statusText;
  String _alertMessage;

  // Interface HomieNode
  bool handleBroadcast(const String& level, const String& value);
  
    // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) override;

  // Interface OLEDStatusIndicator
  virtual void drawOverlay(OLEDDisplay& display,  OLEDDisplayUiState& state, uint8_t idx);
  
  void drawWifiStrength(OLEDDisplay& display);

public:
  StatusNode(const char *name);

  void event(const HomieEvent& event);
  
  void setStatusText(String const value) {
    _statusText = value;
  };
  bool isAlert() {
    return _alert;
  }
  bool isWifiConnected() {
    return _wifi;
  };
};

#endif
