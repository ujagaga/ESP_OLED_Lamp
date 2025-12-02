#ifndef NTPSYNC_H
#define NTPSYNC_H

#include <Arduino.h>

void NTPS_init(void);                 // call once at startup
void NTPS_process(void);              // call often from loop()
String NTPS_getHHMM(void);            // returns "HH:MM" (or "--:--" if unknown)
bool NTPS_hasSynced(void);            // true if time is reasonably synced
void NTPS_forceResync(void);          // force NTP re-init (resync)
bool NTPS_isReadyForDisplay(void);

#endif
