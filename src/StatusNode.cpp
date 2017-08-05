/*
 * A Homie node that shows (Homie) status information
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include <Homie.hpp>
#include "StatusNode.hpp"

StatusNode::StatusNode(const char *name) :
  HomieNode(name, "test"),
  _name(name),_wifi(false), _mqtt(false),	_cfgmode(false), _alert(false) {
};

bool StatusNode::handleBroadcast(const String& level, const String& value) {
	if (level.equals("alert")) {
		_alert = true;
		_alertMessage = value;
		return true;
	}
	return false;
}

void StatusNode::Event(const HomieEvent& event) {
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

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(x, 0, _name);

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
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 10, _statusText);
};

void StatusNode::setup() {
  Homie.getLogger() << "StatusNode Setup" << endl;
};

void StatusNode::loop() {

};
