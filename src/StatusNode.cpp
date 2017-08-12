/*
 * A Homie node that shows (Homie) status information
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include <Homie.hpp>
#include "StatusNode.hpp"

HomieSetting<long> timeclientOffset("TcOffset", "The time zone offset for the NTP client in hours (-12 .. 12");
HomieSetting<long> timeclientUpdate("TcUpdate", "The update interval in minutes for the NTP client (must be at least 10 minutes)");

StatusNode::StatusNode(const char *name) :
  HomieNode(name, "test") {
  _name = name;
  _wifi = false;
  _mqtt = false;  
  _cfgmode = false; 
  _alert = false;
  _timeClient = new NTPClient(_ntpUDP, TC_SERVER);
};

bool StatusNode::handleBroadcast(const String& level, const String& value) {
  if (level.equals("alert")) {
    _alert = true;
    _alertMessage = value;
    return true;
  }
  return false;
}

void StatusNode::event(const HomieEvent& event) {
  _cfgmode = false;
  switch (event.type) {
  case HomieEventType::STANDALONE_MODE:
    _statusText = "Standalone mode";
    break;
  case HomieEventType::CONFIGURATION_MODE:
    _statusText = "Configuration mode";
    _cfgmode = true;
    break;
  case HomieEventType::NORMAL_MODE:
    _statusText = "Normal mode";
    break;
  case HomieEventType::OTA_STARTED:
    _statusText = "OTA started";
    break;
  case HomieEventType::OTA_FAILED:
    _statusText = "OTA failed";
    break;
  case HomieEventType::OTA_SUCCESSFUL:
    _statusText = "OTA successful";
    break;
  case HomieEventType::ABOUT_TO_RESET:
    _statusText = "About to reset";
    break;
  case HomieEventType::READY_TO_SLEEP:
    _statusText = "Ready to sleep" ;
    break;
   case HomieEventType::WIFI_CONNECTED:
    _statusText = "Wi-Fi connected"; // + event.ip;
    _wifi = true;
    break;
  case HomieEventType::WIFI_DISCONNECTED:
    _statusText = "Wi-Fi disconnected";
    _wifi = false;
    break;
  case HomieEventType::MQTT_READY:
    _statusText = "MQTT ready";
    _mqtt = true;
    break;
  case HomieEventType::MQTT_DISCONNECTED:
    _statusText = "MQTT disconnected";
    _mqtt = false;
    break;
  case HomieEventType::MQTT_PACKET_ACKNOWLEDGED:
    _statusText = "";//"MQTT packet acknowledged"; //, packetId: " + event.packetId;
    break;
  }
  // Homie.getLogger() << "Event: " <<  _statusText << endl;
}

// Interface OLEDFrame
void StatusNode::drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) {
  uint32_t now = millis();

  // display.setFont(ArialMT_Plain_10);
  // display.setTextAlignment(TEXT_ALIGN_LEFT);
  // display.drawString(x, 0, _name);

  if (_cfgmode) {
  }
  else if (_wifi) {
    if (!_mqtt)
      _statusText = "Connecting to Mqtt";
  } else {
    _statusText = "Connecting to WiFi";
    uint8_t cycle3 = (now >> 4) % 3;
    display.drawXbm(46, 30, 8, 8, cycle3 == 0 ? activeSymbol : inactiveSymbol);
    display.drawXbm(60, 30, 8, 8, cycle3 == 1 ? activeSymbol : inactiveSymbol);
    display.drawXbm(74, 30, 8, 8, cycle3 == 2 ? activeSymbol : inactiveSymbol);
  }

  // draw status text at center
  if (Homie.isConnected()) {
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(x + 64, y + 15, _statusText);
  }
};

void StatusNode::drawOverlay(OLEDDisplay& display, OLEDDisplayUiState& state, uint8_t idx) {
  
  display.drawHorizontalLine(0, 52, 128);
  display.setColor(WHITE);
  display.setFont(ArialMT_Plain_10);

  // draw WiFi symbol at bottom right
  if (_cfgmode) {
    drawWifiStrength(display);
  }
  else if (_wifi) {
    drawWifiStrength(display);
  } else {
    // do whatever you like while connecting to WiFi
  }

  // draw status text at center
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 54, _statusText);

  if (_alert) {
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 0, "!!! ALARM !!!");
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.setFont(ArialMT_Plain_24);
    display.drawStringMaxWidth(64,32,128,_alertMessage);
  }
}
  
void StatusNode::drawWifiStrength(OLEDDisplay& display) {
  // converts the dBm to a range between 0 and 100%
  int32_t dbm = WiFi.RSSI();
  int8_t wifiQuality = -1;

  if (dbm <= -100) {
    wifiQuality = 0;
  } else if (dbm >= -50) {
    wifiQuality = 100;
  } else {
    wifiQuality = 2 * (dbm + 100);
  }

  for (int8_t i = 0; i < 4; i++) {
    for (int8_t j = 0; j < 2 * (i + 1); j++) {
      if ((wifiQuality > i * 25) || (j == 0)) {
        display.setPixel(120 + 2 * i, 63 - j);
      }
    }
  }
}

void StatusNode::beforeSetup() {
  Homie.getLogger() << "• StatusNode - Before setup" << endl;
  timeclientOffset.setDefaultValue(TC_TIMEZONEOFFSET);
  timeclientUpdate.setDefaultValue(TC_UPDATEINTERVAL).setValidator([] (long candidate) {
    return (candidate >= 10) && (candidate <= 24*6*10); // Update interval etween 10 minutes and 24 hours
  });
}

void StatusNode::setupHandler() {
  Homie.getLogger() << "• StatusNode - Setuphandler" << endl
                    << "  ◦ Time zone offset: UTC " << timeclientOffset.get() " hours" << endl
                    << "  ◦ Update interval : " << timeclientUpdate.get() << " minutes" << endl;
  _timeClient->setTimeOffset(timeclientOffset.get() * 3600UL);
  _timeClient->setUpdateInterval(timeclientUpdate.get() * 60000UL);
  _timeClient->begin();
};

void StatusNode::loop() {
  if (_timeClient->update()) {
    setStatusText(_timeClient->getFormattedTime());
  }
};
