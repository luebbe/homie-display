/*
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#define FW_NAME "display"
#define FW_VERSION "1.3.a"

#include <Homie.h>

#include "ota.hpp"
#include "welcome.hpp"
#include "MqttCollector.hpp"

MqttCollector mqttCollector("MqttFrame", 0);

// Display & UI
#include <SSD1306.h>
// #include <OLEDDisplayUi.h>

const int I2C_DISPLAY_ADDRESS = 0x3c;
const int PIN_SDA = 4; // =D2 on Wemos
const int PIN_SCL = 5; // =D1 on Wemos

// Connected peripherals
SSD1306Wire display(I2C_DISPLAY_ADDRESS, PIN_SDA, PIN_SCL);
// OLEDDisplayUi ui(&display);

OtaDisplaySSD1306 ota(display, NULL);
WelcomeSSD1306 welcome(display, FW_NAME, FW_VERSION);

// void resumeTransition()
// {
//   ui.enableAutoTransition();
//   Homie.getLogger() << endl;
// }

// void stopTransition()
// {
//   ui.disableAutoTransition();
//   ui.switchToFrame(0);
//   Homie.getLogger() << endl;
// }

// bool displayNodeInputHandler(const HomieRange &range, const String &value)
// {
//   Homie.getLogger() << "Display on: " << value << endl;
//   if (value != "true" && value != "false")
//     return false;

//   if (value == "true")
//     display.displayOn();
//   else
//     display.displayOff();
//   return true;
// }

// void onHomieEvent(const HomieEvent &event)
// {
//   switch (event.type)
//   {
//   case HomieEventType::WIFI_CONNECTED:
//     resumeTransition();
//     break;
//   case HomieEventType::WIFI_DISCONNECTED:
//     stopTransition();
//     break;
//   default:;
//     break;
//   }
//   // dispatch event
// }

void loopHandler()
{
  ota.loop();
}

void setupHandler()
{
  // Called after WiFi is connected
  Homie.getLogger() << "Setup handler" << endl;
  mqttCollector.onReadyToOperate();
}

void setup()
{
  Homie_setFirmware(FW_NAME, FW_VERSION);

  Serial.begin(SERIAL_SPEED);
  Serial << endl
         << endl;

  welcome.show();
  ota.setup();

  mqttCollector.beforeSetup();

  // Initializes I2C for BME280 sensor and display
  Homie.getLogger() << "• Wire begin SDA=" << PIN_SDA << " SCL=" << PIN_SCL << endl;
  Wire.begin(PIN_SDA, PIN_SCL);

  // Display and UI
  // ui.setTargetFPS(30);
  // ui.setFrameAnimation(SLIDE_LEFT);
  // ui.setTimePerFrame(5000);
  // ui.setTimePerTransition(2000);
  // ui.disableAutoTransition();
  // ui.disableAllIndicators();
  // ui.init();
  display.flipScreenVertically();
  display.setColor(WHITE);
  display.display();

  // Homie.onEvent(onHomieEvent);

  Homie.disableResetTrigger();
  // Homie.disableLedFeedback();
  Homie.setLoopFunction(loopHandler);
  Homie.setSetupFunction(setupHandler);
  Homie.setup();
}

void loop()
{
  Homie.loop();
  // ui.update();
}
