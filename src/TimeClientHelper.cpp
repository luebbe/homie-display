/*
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "TimeClientHelper.hpp"

// NTP Client
const char *TC_SERVER = "europe.pool.ntp.org";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, TC_SERVER);

// For starters use hardwired Central European Time (Berlin, Paris, ...)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120}; // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};   // Central European Standard Time
Timezone timeZone(CEST, CET);

void timeClientSetup()
{
  // initialize NTP Client
  timeClient.begin();

  // Set callback for time library and leave the sync to the NTP client
  setSyncProvider(getNtpTime);
  setSyncInterval(0);
}

time_t getNtpTime()
{
  if (timeClient.update())
  {
    // Always return the time for the current time zone
    return timeZone.toLocal(timeClient.getEpochTime());
  }
}

String getFormattedTime(time_t rawTime)
{
  unsigned long hours = (rawTime % 86400L) / 3600;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = (rawTime % 3600) / 60;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  unsigned long seconds = rawTime % 60;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
}

