#ifndef NTPSYNC_H
#define NTPSYNC_H

#include <Arduino.h>

void NTPS_process(void);              // call often from loop()
bool NTPS_hasSynced(void);            // true if time is reasonably synced
String NTPS_getHHMM(void);            // returns "HH:MM" (or "--:--" if unknown)
String NTPS_getHH(void);
String NTPS_getMM(void);
String NTPS_getDate(void);
void NTPS_init(void);

#endif
