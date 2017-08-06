/*
 * Base class for weather underground display frames
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#ifndef SRC_WU_FRAME_H_
#define SRC_WU_FRAME_H_

#include <OLEDIndexFrame.hpp>
#include "WundergroundClient.h"
#include "WeatherStationFonts.h"

class WuFrame: public OLEDIndexFrame {
protected:
  WundergroundClient *_wuClient;
public:
  WuFrame(WundergroundClient *wuClient);
};

#endif
