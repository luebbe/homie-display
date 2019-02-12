/*
 * Base class for weather display frames
 * Version: 2.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "WeatherFrame.hpp"

WuFrame::WuFrame(WundergroundClient *wuClient) : _wuClient(wuClient)
{
}
