/*
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <NTPClient.h>
#include <WifiUdp.h>
#include "Time.h"
#include "Timezone.h"

struct TimeZoneInfo
{
  char description[21]; // 20 chars max
  Timezone *timezone;
};

void timeClientSetup();
int getTzCount();
time_t getUtcTime();
time_t getTimeFor(int index, TimeChangeRule **tcr);
String getTimeInfoFor(int index);
String getFormattedTime(time_t rawTime);
