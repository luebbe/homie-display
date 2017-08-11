/*
 * A Homie node that shows (Homie) status information
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#ifndef SRC_STATUSNODE_H_
#define SRC_STATUSNODE_H_

#include <NTPClient.h>
#include <OLEDIndexFrame.hpp>
#include "images.h"

class StatusNode: public HomieNode, public OLEDIndexFrame {
private:
  const long TC_TIMEZONEOFFSET = 0;  // Default UTC
  const long TC_UPDATEINTERVAL = 15; // Default update every 15 minutes
  const char *TC_SERVER = "europe.pool.ntp.org";
  const char *_name;
  bool _cfgmode;
  bool _mqtt;
  bool _wifi;
  bool _alert;
  String _statusText;
  String _alertMessage;
  WiFiUDP _ntpUDP;
  NTPClient *_timeClient;

  void drawWifiStrength(OLEDDisplay& display);

protected:
  virtual void setup() override;
  virtual void loop() override;

public:
  StatusNode(const char *name);

  // Interface HomieNode
  void Event(const HomieEvent& event);
  bool handleBroadcast(const String& level, const String& value);

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) override;

  // Interface OLEDStatusIndicator
  virtual void drawOverlay(OLEDDisplay& display,  OLEDDisplayUiState& state, uint8_t idx);
  
  void beforeSetup();
  
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
