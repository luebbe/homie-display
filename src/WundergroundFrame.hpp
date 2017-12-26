/*
 * Base class for weather underground display frames
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <OLEDIndexFrame.hpp>
#include <NTPClient.h>

#include "WundergroundClient.h"
#include "WeatherStationFonts.h"

class WuFrame : public OLEDIndexFrame
{
protected:
  WundergroundClient *_wuClient;
  NTPClient _timeClient;

public:
  WuFrame(WundergroundClient *wuClient, NTPClient timeClient);
};
