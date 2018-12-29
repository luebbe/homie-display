/*
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#define FW_NAME "display"
#define FW_VERSION "1.0.7"

#include <Homie.h>

#include "ota.hpp"
#include "welcome.hpp"
#include "StatusNode.hpp"
#include "MqttNode.hpp"
#include "WundergroundNode.hpp"
#include "TimeClientHelper.hpp"

// Display & UI
#include <SSD1306.h>
#include <OLEDDisplayUi.h>

const int I2C_DISPLAY_ADDRESS = 0x3c;
const int PIN_SDA = 4; // =D2 on Wemos
const int PIN_SCL = 5; // =D1 on Wemos

// Connected peripherals
SSD1306Wire display(I2C_DISPLAY_ADDRESS, PIN_SDA, PIN_SCL);
OLEDDisplayUi ui(&display);

OtaDisplaySSD1306 ota(display, NULL);
WelcomeSSD1306 welcome(display, FW_NAME, FW_VERSION);

StatusNode statusNode("Status", FW_NAME, FW_VERSION);
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
  default:;
    break;
  }
  // dispatch event
  statusNode.event(event);
}

void loopHandler()
{
  // Don't rotate screens when an alert is shown
  if (statusNode.isAlert())
  {
    stopTransition();
  }

  ota.loop();
}

void setupHandler()
{
  // Called after WiFi is connected
  Homie.getLogger() << "Setup handler" << endl;

  timeClientSetup();

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

  // Initializes I2C for BME280 sensor and display
  Homie.getLogger() << "• Wire begin SDA=" << PIN_SDA << " SCL=" << PIN_SCL << endl;
  Wire.begin(PIN_SDA, PIN_SCL);

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
