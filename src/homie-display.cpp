/*
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#define FW_NAME "display"
#define FW_VERSION "1.0.7"

#include <Homie.h>
#include <NTPClient.h>

#include "ota.hpp"
#include "welcome.hpp"
#include "StatusNode.hpp"
#include "MqttNode.hpp"
#include "WundergroundNode.hpp"
#include "Time.h"
#include "Timezone.h"

// NTP Client
const char *TC_SERVER = "europe.pool.ntp.org";
const long TC_TIMEZONEOFFSET = 0;  // Default UTC
const long TC_UPDATEINTERVAL = 15; // Default update every 15 minutes

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, TC_SERVER);

// For starters use hardwired Central European Time (Berlin, Paris, ...)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120}; // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};   // Central European Standard Time
Timezone timeZone(CEST, CET);

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

HomieSetting<long> timeclientUpdate("TcUpdate", "The update interval in minutes for the NTP client (must be at least 10 minutes)");

int _yesterday = -1;

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

time_t getNtpTime()
{
  if (timeClient.update())
  {
    // Always return the time for the current time zone
    return timeZone.toLocal(timeClient.getEpochTime());
  }
}

String getFormattedTime(time_t rawTime)
{
  unsigned long hours = (rawTime % 86400L) / 3600;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = (rawTime % 3600) / 60;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  unsigned long seconds = rawTime % 60;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
}

void loopHandler()
{
  String curTime = getFormattedTime(now());
  statusNode.setStatusText(curTime);

  // Reset min/max values at midnight
  if (day() > _yesterday)
  {
    _yesterday = day();
    mqttNode.resetMinMax();
  }

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
  Homie.getLogger() << "Setuphandler" << endl
                    << "• Update interval : " << timeclientUpdate.get() << " minutes" << endl;

  // initialize NTP Client
  timeClient.setUpdateInterval(timeclientUpdate.get() * 60000UL);
  timeClient.begin();

  // Set callback for time library and leave the sync to the NTP client
  setSyncProvider(getNtpTime);
  setSyncInterval(0);
  _yesterday = day();

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
