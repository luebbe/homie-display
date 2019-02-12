/*
 * Base class for weather display frames
 * Version: 2.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <OLEDIndexFrame.hpp>
#include "TimeLib.h"
#include "WundergroundClient.h"
#include "WeatherStationFonts.h"

class WuFrame : public OLEDIndexFrame
{
protected:
  WundergroundClient *_wuClient;

public:
  explicit WuFrame(WundergroundClient *wuClient);
};
