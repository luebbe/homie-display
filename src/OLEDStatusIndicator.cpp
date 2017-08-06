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

#include "OLEDStatusIndicator.hpp"

#include <Homie.h>
#include <ESP8266WiFi.h> // Needed for WiFi Signal Strength

OLEDStatusIndicator::OLEDStatusIndicator() :
    _wifi(false), _mqtt(false),  _cfgmode(false), _alert(false) {
}

bool OLEDStatusIndicator::handleBroadcast(const String& level, const String& value) {
  if (level.equals("alert")) {
    _alert = true;
    _alertMessage = value;
    return true;
  }
  return false;
}

void OLEDStatusIndicator::Event(const HomieEvent& event) {
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
    _statusText = "";
    //_statusText = "MQTT packet acknowledged"; //, packetId: " + event.packetId;
    break;
  }
  // Homie.getLogger() << "Event: " <<  _statusText << endl;
}

void OLEDStatusIndicator::drawOverlay(OLEDDisplay& display, OLEDDisplayUiState& state, uint8_t idx) {

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
    display.drawString(32, 0, "!!! ALARM !!!");
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.setFont(ArialMT_Plain_24);
    display.drawStringMaxWidth(64,32,128,_alertMessage);
  }
}

void OLEDStatusIndicator::drawWifiStrength(OLEDDisplay& display) {
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
