/*
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#define FW_NAME "display"
#define FW_VERSION "1.0.0"

#include <Homie.h>

#include "homie-node-collection.h"
#include "StatusNode.hpp"
#include "MqttNode.hpp"
#include "WundergroundNode.hpp"

// Display & UI
#include <SSD1306.h>
#include <OLEDDisplayUi.h>
#include <OLEDStatusIndicator.hpp>

#define SERIAL_SPEED 115200

const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = 12;
const int SCL_PIN = 13;

// Connected peripherals
SSD1306Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN);
OtaDisplay ota(&display);
OLEDDisplayUi ui(&display);

StatusNode statusNode("Status");
MqttNode nodeNode1("Test Node 1");
WundergroundNode wundergroundNode("Wunderground");

void resumeTransition() {
  ui.enableAutoTransition();
}

void stopTransition() {
  ui.disableAutoTransition();
  ui.switchToFrame(0);
}

void onHomieEvent(const HomieEvent& event) {
  switch (event.type) {
  case HomieEventType::WIFI_CONNECTED:
    resumeTransition();
  break;
  case HomieEventType::WIFI_DISCONNECTED:
    stopTransition();
  break;
  }
  // dispatch event
  statusNode.Event(event);
}

void loopHandler() {
  if (statusNode.isAlert()) {
    stopTransition();
  }
  ota.loop();
}

void setupHandler() {
  // Called after WiFi is connected
  Homie.getLogger() << "Setuphandler" << endl;
  wundergroundNode.setupHandler();
}

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial << endl << endl;

  welcome();
  ota.setup();

  nodeNode1.beforeSetup();
  statusNode.beforeSetup();
  wundergroundNode.beforeSetup();

  // Display and UI
  ui.setTargetFPS(10);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.disableAutoTransition();
  ui.disableAllIndicators();
  ui.init();
  display.flipScreenVertically();

  Homie_setFirmware(FW_NAME, FW_VERSION);

  Homie.onEvent(onHomieEvent);

  Homie.disableResetTrigger();
  Homie.disableLedFeedback();
  Homie.setLoopFunction(loopHandler);
  Homie.setSetupFunction(setupHandler);
  Homie.setup();
}

void loop() {
  Homie.loop();
  ui.update();
}
