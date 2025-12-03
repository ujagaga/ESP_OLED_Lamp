#ifndef NTPSYNC_H
#define NTPSYNC_H

#include <Arduino.h>

void NTPS_init(void);                 // call once at startup
void NTPS_process(void);              // call often from loop()
bool NTPS_hasSynced(void);            // true if time is reasonably synced
String NTPS_getHHMM(void);            // returns "HH:MM" (or "--:--" if unknown)
String NTPS_getHH(void);
String NTPS_getMM(void);

#endif
