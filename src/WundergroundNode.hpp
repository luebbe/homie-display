/*
 * Homie node that fetches data from weather underground
 * and shows it on an Oled display
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#ifndef SRC_WUNDERGROUNDNODE_H_
#define SRC_WUNDERGROUNDNODE_H_

#include <HomieNode.hpp>
#include <NTPClient.h>

#include "WundergroundClient.h"
#include "WundergroundCurrentWeatherFrame.hpp"
#include "WundergroundForecastFrame.hpp"

class WundergroundNode: public HomieNode {
private:
  const boolean IS_METRIC = true;
  const char * WUNDERGRROUND_API_KEY = "YOUR_API_KEY";
  const char * WUNDERGRROUND_LANGUAGE = "EN";
  const char * WUNDERGROUND_COUNTRY = "UK";
  const char * WUNDERGROUND_CITY = "YOURTOWN";
  const long WUNDERGROUND_UPDATE = 15; // Default update every 15 minutes
  WundergroundClient *_wuClient;
  WuCurrentWeatherFrame *_wuCurrent;
  WuForecastFrame *_wuForecast;
  unsigned long _nextUpdate;
  bool isConfigured();
protected:
  virtual void loop() override;
public:
  WundergroundNode(const char *name, NTPClient timeClient);
  void beforeSetup();
  void setupHandler();
};

#endif
