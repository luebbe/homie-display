/*
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#define FW_NAME "display"
#define FW_VERSION "1.0.6"

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
const int PIN_SDA = 4; // =D2 on Wemos
const int PIN_SCL = 5; // =D1 on Wemos

// Connected peripherals
SSD1306Wire display(I2C_DISPLAY_ADDRESS, PIN_SDA, PIN_SCL);
OLEDDisplayUi ui(&display);

OtaDisplaySSD1306 ota(display, NULL);
WelcomeSSD1306 welcome(display, FW_NAME, FW_VERSION);

StatusNode statusNode("Status", FW_NAME, FW_VERSION);
MqttNode mqttNode("MqttClient");
WundergroundNode wundergroundNode("Wunderground", &timeClient);

HomieSetting<long> timeclientOffset("TcOffset", "The time zone offset for the NTP client in hours (-12 .. 12");
HomieSetting<long> timeclientUpdate("TcUpdate", "The update interval in minutes for the NTP client (must be at least 10 minutes)");

bool _resetMinMax = false;

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
    String curTime = timeClient.getFormattedTime();
    statusNode.setStatusText(curTime);
    int secondsOfDay = timeClient.getEpochTime() % 86400;
    if (!_resetMinMax)
    {
      // Reset min/max values at midnight
      if (secondsOfDay == 0)
      {
        _resetMinMax = true;
        mqttNode.resetMinMax();
      }
    }
    else if (secondsOfDay == 10)
    {
      // Rearm trigger ten seconds after midnight
      _resetMinMax = false;
    }
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

  // Initializes I2C for BME280 sensor and display
  Homie.getLogger() << "• Wire begin SDA=" << PIN_SDA << " SCL=" << PIN_SCL << endl;
  Wire.begin(PIN_SDA, PIN_SCL);

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
