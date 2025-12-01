#include "NTPSync.h"
#include "config.h"

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <time.h>

static bool tzFetchedThisBoot = false;      // we fetch TZ only once per boot
static bool ntpSynced = false;
static unsigned long lastNtpInitMillis = 0;
static unsigned long lastTimeCheckMillis = 0;

// helper: extract a JSON string value (very small manual parser)
// looks for: "key":"value"
static String extractJsonString(const String& payload, const char* key) {
  String needle = String("\"") + key + "\":\"";
  int idx = payload.indexOf(needle);
  if (idx < 0) {
    return String();
  }
  idx += needle.length();
  int idx2 = payload.indexOf('"', idx);
  if (idx2 < 0) return String();
  return payload.substring(idx, idx2);
}

// helper: check presence of boolean field: "dst":true or false
static bool extractJsonBool(const String& payload, const char* key) {
  String needle = String("\"") + key + "\":";
  int idx = payload.indexOf(needle);
  if (idx < 0) return false;
  idx += needle.length();
  // look for "true"
  if (payload.indexOf("true", idx) >= 0) return true;
  return false;
}

// Build a simple POSIX TZ string based on utc_offset and whether DST likely exists.
// This is intentionally small: it handles typical European offsets (CET/CEST/EET) and falls back to UTC0.
static String buildPosixTZ(const String& utc_offset, bool dst) {
  // utc_offset example: "+01:00" or "-05:00"
  if (utc_offset.length() < 3) return String("UTC0");

  int sign = (utc_offset.charAt(0) == '-') ? -1 : 1;
  int hours = utc_offset.substring(1, 3).toInt() * sign;

  // POSIX invert sign convention (east is negative): e.g. UTC+1 => POSIX "UTC-1" or CET-1
  int posixOffset = -hours;

  // Determine base name
  String baseName = "UTC";
  if (hours == 1) baseName = "CET";   // UTC+1
  else if (hours == 2) baseName = "EET"; // UTC+2
  else if (hours == 0) baseName = "UTC";
  else if (hours == -5) baseName = "EST"; // examples, not exhaustive

  String tz = baseName;
  // Add numeric offset (POSIX style)
  if (posixOffset >= 0) tz += "+" + String(posixOffset);
  else tz += "-" + String(abs(posixOffset));

  // If DST is indicated, append a common EU DST rule if base was CET
  if (dst) {
    // Use typical EU DST rules for CET -> CEST
    if (baseName == "CET") {
      tz += "CEST,M3.5.0/2,M10.5.0/3"; // last Sun March -> last Sun Oct
    } else if (baseName == "EET") {
      tz += "EEST,M3.5.0/3,M10.5.0/4";
    } else {
      // Generic suffix for other zones (best-effort)
      tz += "DST,M3.5.0/2,M10.5.0/3";
    }
  }

  return tz;
}

// Try to fetch timezone info from WorldTimeAPI and apply it immediately.
// This is called once (first time WiFi is connected). It blocks for the HTTP request only.
static void fetchAndApplyTimezoneOnce() {
  if (tzFetchedThisBoot) return;
  if (WiFi.status() != WL_CONNECTED) return;

  Serial.println("NTPS: Fetching timezone from worldtimeapi...");

  WiFiClient client;
  HTTPClient http;
  String url = String("http://worldtimeapi.org/api/timezone/") + REGION;

  if (!http.begin(client, url)) {
    Serial.println("NTPS: http.begin() failed");
    return;
  }

  int code = http.GET();
  if (code != 200) {
    Serial.printf("NTPS: HTTP GET failed: %d\n", code);
    http.end();
    return;
  }

  String payload = http.getString();
  http.end();

  String utc_offset = extractJsonString(payload, "utc_offset"); // "+01:00"
  bool dst = extractJsonBool(payload, "dst");

  Serial.printf("NTPS: got utc_offset=%s dst=%d\n", utc_offset.c_str(), dst ? 1 : 0);

  String posix = buildPosixTZ(utc_offset, dst);
  Serial.printf("NTPS: Applying POSIX TZ: %s\n", posix.c_str());

  // Apply TZ and start SNTP
  setenv("TZ", posix.c_str(), 1);
  tzset();

  const char* ntp1 = "pool.ntp.org";
  const char* ntp2 = "time.nist.gov";
  configTime(0, 0, ntp1, ntp2);

  lastNtpInitMillis = millis();
  tzFetchedThisBoot = true;

  // quick loop to check whether SNTP sets time soon (non-blocking mostly)
  unsigned long start = millis();
  while (millis() - start < 5000UL) {
    time_t now = time(nullptr);
    if (now > 100000) {
      ntpSynced = true;
      Serial.println("NTPS: initial sync OK");
      break;
    }
    delay(200);
  }
  if (!ntpSynced) {
    Serial.println("NTPS: initial SNTP not yet synced (will continue in background)");
  }
}

// Initialize NTP subsystem at boot.
// This sets a safe default TZ (UTC) and configTime so that even without internet you have monotonic time functionality.
void NTPS_init(void) {
  Serial.println("NTPS: init - applying fallback UTC and starting NTP client");
  // Start with UTC fallback until we fetch proper TZ
  setenv("TZ", "UTC0", 1);
  tzset();

  const char* ntp1 = "pool.ntp.org";
  const char* ntp2 = "time.nist.gov";
  configTime(0, 0, ntp1, ntp2);

  lastNtpInitMillis = millis();
  tzFetchedThisBoot = false;
  ntpSynced = false;
}

// Call frequently from loop(). Will trigger timezone fetch once when WiFi is up and will re-init NTP every NTP_SYNC_H hours.
void NTPS_process(void) {
  // Trigger timezone fetch once after WiFi connects
  if (!tzFetchedThisBoot && WiFi.status() == WL_CONNECTED) {
    fetchAndApplyTimezoneOnce();
  }

  // Periodically check whether time got synced (once per second)
  unsigned long nowMs = millis();
  if (nowMs - lastTimeCheckMillis > 1000UL) {
    lastTimeCheckMillis = nowMs;
    time_t nowSec = time(nullptr);
    if (nowSec > 100000) ntpSynced = true;
  }

  // Periodic forced re-init/resync every NTP_SYNC_H hours
  unsigned long intervalMs = (unsigned long)NTP_SYNC_H * 3600UL * 1000UL;
  if (intervalMs == 0) intervalMs = 4UL * 3600UL * 1000UL; // safety
  if (nowMs - lastNtpInitMillis >= intervalMs) {
    Serial.println("NTPS: periodic NTP re-init");
    const char* ntp1 = "pool.ntp.org";
    const char* ntp2 = "time.nist.gov";
    configTime(0, 0, ntp1, ntp2);
    lastNtpInitMillis = nowMs;
    // keep tzFetchedThisBoot true so we don't attempt fetch again this boot
  }
}

// Force immediate NTP re-init (can be called from UI)
void NTPS_forceResync(void) {
  const char* ntp1 = "pool.ntp.org";
  const char* ntp2 = "time.nist.gov";
  configTime(0, 0, ntp1, ntp2);
  lastNtpInitMillis = millis();
  Serial.println("NTPS: forced NTP re-init requested");
}

// Return whether we have a reasonable time
bool NTPS_hasSynced(void) {
  time_t now = time(nullptr);
  return now > 100000;
}

// Return HH:MM local time string (POSIX TZ applied by setenv/tzset)
String NTPS_getHHMM(void) {
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);
  if (!t) return String("--:--");
  char buf[6];
  snprintf(buf, sizeof(buf), "%02d:%02d", t->tm_hour, t->tm_min);
  return String(buf);
}
