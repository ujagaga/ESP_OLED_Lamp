#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Timezone.h>

// ---- NTP / WiFi config ----
static const char* NTP_SERVERS[] = {
"rs.pool.ntp.org",
"time.cloudflare.com",
"time.google.com"
};
static const unsigned long NTP_UPDATE_INTERVAL_MS = 60 * 1000; // 60s

// ---- NTPClient + Timezone setup ----
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVERS[0], 0, NTP_UPDATE_INTERVAL_MS);

// ---- Serbian Timezone rules ----
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120}; // UTC+2 summer
TimeChangeRule CET = {"CET", Last, Sun, Oct, 3, 60}; // UTC+1 winter
Timezone serbiaTZ(CEST, CET);

static bool ntpSynced = false;

void NTPS_init() {
  ntpSynced = false;
  if (WiFi.status() != WL_CONNECTED) {
      Serial.println("NTPS: WiFi not connected yet, waiting...");
      return;
  }
  Serial.println("NTPS: starting NTP client...");
  timeClient.begin();
  timeClient.update();  
}

void NTPS_process() {
  if (WiFi.status() != WL_CONNECTED) return;

  if (!timeClient.update()) {
      timeClient.forceUpdate();
  }

  // mark synced if epoch is realistic
  if (timeClient.getEpochTime() > 100000) {
    if (!ntpSynced) {
        Serial.println("NTPS: time synced");
        ntpSynced = true;
    }
  } else {
      ntpSynced = false;
  }
}


bool NTPS_hasSynced() {
  return ntpSynced;
}

time_t NTPS_getLocalEpoch() {
  time_t utc = timeClient.getEpochTime();
  return serbiaTZ.toLocal(utc);
}

// -------------------------
// GET HH:MM (zero-padded, localtime)
// -------------------------
String NTPS_getHHMM() {
  time_t local = NTPS_getLocalEpoch();
  struct tm t;
  localtime_r(&local, &t);
  char buf[6];
  snprintf(buf, sizeof(buf), "%02d\n%02d", t.tm_hour, t.tm_min);
  return String(buf);
}

String NTPS_getHH() {
  time_t local = NTPS_getLocalEpoch();
  struct tm t;
  localtime_r(&local, &t);
  char buf[3];
  snprintf(buf, sizeof(buf), "%02d", t.tm_hour);
  return String(buf);
}

String NTPS_getMM() {
  time_t local = NTPS_getLocalEpoch();
  struct tm t;
  localtime_r(&local, &t);
  char buf[3];
  snprintf(buf, sizeof(buf), "%02d", t.tm_min);
  return String(buf);
}

String NTPS_getDate() {
  time_t local = NTPS_getLocalEpoch();
  struct tm t;
  localtime_r(&local, &t);
  char buf[6];
  snprintf(buf, sizeof(buf), "%02d.%02d", t.tm_mday, t.tm_mon + 1); // tm_mon is 0-based
  return String(buf);
}
