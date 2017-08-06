/*
 * OLEDStatusIndicator.cpp
 *
 *  An overlay for other display frames

 *  Created on: 14.08.2016
 *      Author: ian
 *      Modifications by:
 *        Lübbe Onken (http://github.com/luebbe)
 *
 *  Shows homie connnection status
 *  Shows WiFi signal stength in bottom right corner, if connected
 */

#ifndef SRC_OLEDSTATUSINDICATOR_H_
#define SRC_OLEDSTATUSINDICATOR_H_

#include <Homie.h>
#include <SSD1306.h>

#include <OLEDDisplayUi.h>
#include <OLEDOverlay.h>

class OLEDStatusIndicator : public OLEDOverlay {
private:
  bool _cfgmode;
  bool _mqtt;
  bool _wifi;
  bool _alert;
  String _statusText;
  String _alertMessage;

public:
  //OLEDStatusIndicator(SSD1306& display); // TODO: Allow to set reference to Display (or abstraction layer)
  OLEDStatusIndicator();
  void Event(const HomieEvent& event);

  bool handleBroadcast(const String& level, const String& value);

  virtual void drawOverlay(OLEDDisplay& display,  OLEDDisplayUiState& state, uint8_t idx);
  void drawWifiStrength(OLEDDisplay& display);

  void setStatusText(String const value) {
    _statusText = value;
  };
  bool isWifiConnected() {
    return _wifi;
  };
};

#endif /* SRC_OLEDSTATUSINDICATOR_H_ */
