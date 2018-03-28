/*
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <NTPClient.h>
#include <WifiUdp.h>
#include "Time.h"
#include "Timezone.h"

void timeClientSetup();
time_t getNtpTime();
String getFormattedTime(time_t rawTime);
