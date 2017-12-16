/*
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#define FW_NAME "display"
#define FW_VERSION "1.0.1"

#include <Homie.h>
#include <NTPClient.h>

#include "ota.hpp"
#include "welcome.hpp"
#include "StatusNode.hpp"
#include "MqttNode.hpp"
#include "WundergroundNode.hpp"

// NTP Client
const char *TC_SERVER = "europe.pool.ntp.org";
const long TC_TIMEZONEOFFSET = 0;  // Default UTC
const long TC_UPDATEINTERVAL = 15; // Default update every 15 minutes

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, TC_SERVER);

// Display & UI
#include <SSD1306.h>
#include <OLEDDisplayUi.h>

const int I2C_DISPLAY_ADDRESS = 0x3c;
const int PIN_SDA = 5;
const int PIN_SCL = 4;
// const int PIN_SDA = 12;
// const int PIN_SCL = 13;

// Connected peripherals
SSD1306Wire display(I2C_DISPLAY_ADDRESS, PIN_SDA, PIN_SCL);
OLEDDisplayUi ui(&display);

OtaDisplaySSD1306 ota(display, NULL);
WelcomeSSD1306 welcome(display, FW_NAME, FW_VERSION);

StatusNode statusNode("Status", FW_NAME, FW_VERSION);
MqttNode mqttNode("MqttClient");
WundergroundNode wundergroundNode("Wunderground", timeClient);

HomieSetting<long> timeclientOffset("TcOffset", "The time zone offset for the NTP client in hours (-12 .. 12");
HomieSetting<long> timeclientUpdate("TcUpdate", "The update interval in minutes for the NTP client (must be at least 10 minutes)");

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
  if (timeClient.update())
  {
    statusNode.setStatusText(timeClient.getFormattedTime());
  }
  if (statusNode.isAlert())
  {
    stopTransition();
  }
  ota.loop();
}

void setupHandler()
{
  // Called after WiFi is connected
  Homie.getLogger() << "Setuphandler" << endl
                    << "• Time zone offset: UTC " << timeclientOffset.get() << " hours" << endl
                    << "• Update interval : " << timeclientUpdate.get() << " minutes" << endl;
  timeClient.setTimeOffset(timeclientOffset.get() * 3600UL);
  timeClient.setUpdateInterval(timeclientUpdate.get() * 60000UL);
  timeClient.begin();

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

  mqttNode.beforeSetup();
  wundergroundNode.beforeSetup();

  timeclientOffset.setDefaultValue(TC_TIMEZONEOFFSET);
  timeclientUpdate.setDefaultValue(TC_UPDATEINTERVAL).setValidator([](long candidate) {
    return (candidate >= 10) && (candidate <= 24 * 6 * 10); // Update interval etween 10 minutes and 24 hours
  });

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
