/*
 * A Homie node that shows (Homie) status information
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include <Homie.hpp>
#include "StatusNode.hpp"

StatusNode::StatusNode(const char *name, const char *fw_name, const char *fw_version)
    : HomieNode(name, "Status")
{
  _name = name;
  _fw_name = fw_name;
  _fw_version = fw_version;
  _wifi = false;
  _mqtt = false;
  _cfgmode = false;
  _alert = false;
  _milliseconds = 0;
  _lasttick = 0;
};

bool StatusNode::handleBroadcast(const String &level, const String &value)
{
  if (level.equals("alert"))
  {
    _alert = true;
    _alertMessage = value;
    return true;
  }
  return false;
}

void StatusNode::event(const HomieEvent &event)
{
  _cfgmode = false;
  switch (event.type)
  {
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
    _statusText = "Ready to sleep";
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
    _statusText = ""; //"MQTT packet acknowledged"; //, packetId: " + event.packetId;
    break;
  }
  // Homie.getLogger() << "Event: " <<  _statusText << endl;
}

void StatusNode::loop()
{
  TimeChangeRule *tcr = NULL;
  _statusText = getFormattedTime(getTimeFor(0, &tcr));
}

// Interface OLEDFrame
void StatusNode::drawFrame(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y)
{
  uint32_t now = millis();
  _milliseconds += (now - _lasttick);
  _lasttick = now;

  if (_cfgmode)
  {
  }
  else if (_wifi)
  {
    if (!_mqtt)
      _statusText = "Connecting to Mqtt";
  }
  else
  {
    _statusText = "Connecting to WiFi";
    uint8_t triCycle = (_milliseconds >> 9) % 3;
    display.drawXbm(46, DISPLAY_HEIGHT * 2 / 3, 8, 8, triCycle == 0 ? activeSymbol : inactiveSymbol);
    display.drawXbm(60, DISPLAY_HEIGHT * 2 / 3, 8, 8, triCycle == 1 ? activeSymbol : inactiveSymbol);
    display.drawXbm(74, DISPLAY_HEIGHT * 2 / 3, 8, 8, triCycle == 2 ? activeSymbol : inactiveSymbol);
  }

  if (Homie.isConnected())
  {
    drawTime(display, state, x, y);
  }
  else
  {
    // show firmware version while not connected
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(x + DISPLAY_WIDTH / 2, y, (String)_fw_name);
    display.drawString(x + DISPLAY_WIDTH / 2, y + DISPLAY_HEIGHT / 3, (String)_fw_version);
  }
};

void StatusNode::drawOverlay(OLEDDisplay &display, OLEDDisplayUiState &state, uint8_t idx)
{

  display.drawHorizontalLine(0, 52, 128);
  display.setColor(WHITE);
  display.setFont(ArialMT_Plain_10);

  // draw WiFi symbol at bottom right
  if (_cfgmode)
  {
    drawWifiStrength(display);
  }
  else if (_wifi)
  {
    drawWifiStrength(display);
  }
  else
  {
    // do whatever you like while connecting to WiFi
  }

  // draw status text at center
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(DISPLAY_WIDTH / 2, 54, _statusText);

  if (_alert)
  {
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(DISPLAY_WIDTH / 2, 0, "!!! ALARM !!!");
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.setFont(ArialMT_Plain_24);
    display.drawStringMaxWidth(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, DISPLAY_WIDTH, _alertMessage);
  }
}

void StatusNode::drawTime(OLEDDisplay &display, OLEDDisplayUiState &state, int16_t x, int16_t y)
{
  if (x > 0)
  {
    if (!_pageSwitched)
    {
      _pageIndex = (_pageIndex + 1) % getTzCount();
      _pageSwitched = true;
    }
  }
  else
  {
    _pageSwitched = false;
  }

  TimeChangeRule *tcr = NULL;
  String tzInfo = getTimeInfoFor(_pageIndex);
  String tzTime = getFormattedTime(getTimeFor(_pageIndex, &tcr));

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(x, y, tzInfo);
  // if ((tcr != NULL) && (x >= 0))
  // {
  //   display.setTextAlignment(TEXT_ALIGN_RIGHT);
  //   display.drawString(x + DISPLAY_WIDTH, y, tcr->abbrev);
  // }
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
  display.drawString(x + DISPLAY_WIDTH / 2, y + 15, tzTime);
}

void StatusNode::drawWifiStrength(OLEDDisplay &display)
{
  // converts the dBm to a range between 0 and 100%
  int32_t dbm = WiFi.RSSI();
  int8_t wifiQuality = -1;

  if (dbm <= -100)
  {
    wifiQuality = 0;
  }
  else if (dbm >= -50)
  {
    wifiQuality = 100;
  }
  else
  {
    wifiQuality = 2 * (dbm + 100);
  }

  for (int8_t i = 0; i < 4; i++)
  {
    for (int8_t j = 0; j < 2 * (i + 1); j++)
    {
      if ((wifiQuality > i * 25) || (j == 0))
      {
        display.setPixel(120 + 2 * i, 63 - j);
      }
    }
  }
}
