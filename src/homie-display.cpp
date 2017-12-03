/*
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#define FW_NAME "display"
#define FW_VERSION "1.0.0"

#include <Homie.h>

// #include "homie-node-collection.h"
#include "ota.hpp"
#include "welcome.hpp"
#include "StatusNode.hpp"
#include "MqttNode.hpp"
#include "WundergroundNode.hpp"

// Display & UI
#include <SSD1306.h>
#include <OLEDDisplayUi.h>

const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = 5;
const int SCL_PIN = 4;
// const int SDA_PIN = 12;
// const int SCL_PIN = 13;

// Connected peripherals
SSD1306Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN);
OtaDisplaySSD1306 ota(display, NULL);
WelcomeSSD1306 welcome(display, FW_NAME, FW_VERSION);
OLEDDisplayUi ui(&display);

StatusNode statusNode("Status");
MqttNode mqttNode("MqttClient");
WundergroundNode wundergroundNode("Wunderground");

void resumeTransition()
{
  ui.enableAutoTransition();
}

void stopTransition()
{
  ui.disableAutoTransition();
  ui.switchToFrame(0);
}

void onHomieEvent(const HomieEvent &event)
{
  switch (event.type)
  {
  case HomieEventType::WIFI_CONNECTED:
    resumeTransition();
    break;
  case HomieEventType::WIFI_DISCONNECTED:
    stopTransition();
    break;
  }
  // dispatch event
  statusNode.event(event);
}

void loopHandler()
{
  if (statusNode.isAlert())
  {
    stopTransition();
  }
  ota.loop();
}

void setupHandler()
{
  // Called after WiFi is connected
  Homie.getLogger() << "Setuphandler" << endl;
  statusNode.setupHandler();
  mqttNode.setupHandler();
  wundergroundNode.setupHandler();
}

void setup()
{
  Serial.begin(SERIAL_SPEED);
  Serial << endl
         << endl;

  welcome.show();
  ota.setup();

  statusNode.beforeSetup();
  mqttNode.beforeSetup();
  wundergroundNode.beforeSetup();

  // Display and UI
  ui.setTargetFPS(30);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.setTimePerFrame(5000);
  ui.setTimePerTransition(1000);
  ui.disableAutoTransition();
  ui.disableAllIndicators();
  ui.init();
  display.flipScreenVertically();
  display.setColor(WHITE);
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 16, (String)FW_NAME + " " + (String)FW_VERSION);
  display.display();

  Homie_setFirmware(FW_NAME, FW_VERSION);

  Homie.onEvent(onHomieEvent);

  Homie.disableResetTrigger();
  Homie.disableLedFeedback();
  Homie.setLoopFunction(loopHandler);
  Homie.setSetupFunction(setupHandler);
  Homie.setup();
}

void loop()
{
  Homie.loop();
  ui.update();
}
