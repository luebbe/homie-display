/*
 * Homie node that fetches data from weather underground
 * and shows it on an Oled display
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#ifndef SRC_WUNDERGROUNDNODE_H_
#define SRC_WUNDERGROUNDNODE_H_

#include <HomieNode.hpp>
#include "WundergroundClient.h"
#include "WundergroundCurrentWeatherFrame.hpp"
#include "WundergroundForecastFrame.hpp"

class WundergroundNode: public HomieNode {
private:
	const int  UPDATE_INTERVAL = 600;
	WundergroundClient *_wuClient;
  WuCurrentWeatherFrame *_wuCurrent;
	WuForecastFrame *_wuForecast;
	unsigned long _updateInterval;
  unsigned long _lastUpdate;
protected:
  virtual void setup() override;
  virtual void loop() override;

public:
	WundergroundNode(const char *name);

	// Interface HomieNode
	// virtual bool handleInput(String const &property, HomieRange range, String const &value) override;

};

#endif
