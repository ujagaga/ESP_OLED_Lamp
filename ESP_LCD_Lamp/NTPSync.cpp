// NTPSync.cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <time.h>

// ---- Config ----
static const char* NTP_SERVERS[] = {
  "rs.pool.ntp.org",
  "time.cloudflare.com",
  "time.google.com"
};
static const unsigned long NTP_RETRY_MS = 120UL * 1000UL;    // try init every 15s until success
static const unsigned long SYNC_CHECK_MS = 120UL * 1000UL;         // check sync every 1s

// ---- State ----
static bool ntpSynced = false;
static unsigned long lastNtpInitMillis = 0;
static unsigned long lastSyncCheckMillis = 0;
static bool ntpInitRequested = false;

// ---- Helpers ----
static void doConfigTimeOnce() {
  // call configTime with two servers (core supports multiple args)
  configTime(0, 0, NTP_SERVERS[0], NTP_SERVERS[1], NTP_SERVERS[2]);
  lastNtpInitMillis = millis();
  ntpInitRequested = true;
  Serial.println("NTPS: doConfigTimeOnce");
}

// -------------------------
//  INIT
// -------------------------
void NTPS_init() {
  // POSIX timezone for Serbia (Europe/Belgrade)
  // CET = UTC+1, CEST = UTC+2 (DST)
  setenv("TZ", "CET-1CEST,M3.5.0/02:00,M10.5.0/03:00", 1);
  tzset();

  ntpSynced = false;
  lastNtpInitMillis = 0;
  lastSyncCheckMillis = 0;
  ntpInitRequested = false;

  // If WiFi is already connected, start NTP immediately,
  // otherwise NTPS_process() will start it once WiFi connects.
  if (WiFi.status() == WL_CONNECTED) {
    doConfigTimeOnce();
  } else {
    Serial.println("NTPS: WiFi not connected yet, will init NTP when WiFi comes up");
  }
}

// -------------------------
//  PROCESS
//  Call this every loop()
// -------------------------
void NTPS_process() {
  unsigned long now = millis();

  // If WiFi is connected and we haven't requested NTP init recently, do it.
  if (WiFi.status() == WL_CONNECTED) {
    if (!ntpInitRequested || (now - lastNtpInitMillis >= NTP_RETRY_MS)) {
      // either not requested yet (first time), or periodic retry
      doConfigTimeOnce();
    }
  } else {
    // If WiFi disconnected, mark that we need to re-init when it reconnects.
    ntpInitRequested = false;
  }

  // Periodically check whether time has been set by SNTP
  if (now - lastSyncCheckMillis < SYNC_CHECK_MS) return;
  lastSyncCheckMillis = now;

  time_t t = time(nullptr);
  // When SNTP syncs, time() will become a realistic epoch > ~100000 (1970+)
  if (t > 100000) {
    if (!ntpSynced) {
      Serial.println("NTPS: time appears synced");
      ntpSynced = true;
    }
  } else {
    // not synced yet
    ntpSynced = false;
  }
}

// -------------------------
//  HAS SYNCED?
// -------------------------
bool NTPS_hasSynced() {
  return ntpSynced;
}

// -------------------------
//  GET HH:MM (zero-padded, localtime)
// -------------------------
String NTPS_getHHMM() {
  time_t now = time(nullptr);
  if (now <= 100000) {
    return String("--\n--");
  }

  struct tm result;
  localtime_r(&now, &result); // thread-safe variant; works on ESP8266 core
  char buf[6];
  snprintf(buf, sizeof(buf), "%02d\n%02d", result.tm_hour + 1, result.tm_min);
  return String(buf);
}

String NTPS_getHH() {
  time_t now = time(nullptr);
  if (now <= 100000) {
    return String("--");
  }

  struct tm result;
  localtime_r(&now, &result); 
  char buf[6] = {0};
  int hour = result.tm_hour + 1;
  if(hour == 24){
    hour = 0;
  }
  snprintf(buf, sizeof(buf), "%02d", hour);
  return String(buf);
}

String NTPS_getMM() {
  time_t now = time(nullptr);
  if (now <= 100000) {
    return String("--");
  }

  struct tm result;
  localtime_r(&now, &result); 
  char buf[6] = {0};
  snprintf(buf, sizeof(buf), "%02d", result.tm_min);
  return String(buf);
}
