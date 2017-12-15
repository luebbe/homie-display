/*
 * Base class for weather underground display frames
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "WundergroundFrame.hpp"

WuFrame::WuFrame(WundergroundClient *wuClient, NTPClient timeClient) : _wuClient(wuClient), _timeClient(timeClient)
{
}
