/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  This is the main sketch file. 
 *  It provides a periodic pooling of other services.
 */

#include "wifi_connection.h"
#include "config.h"
#include "pinctrl.h"
#include "http_server.h"
#include "NTPSync.h"
#include "web_socket.h"


void setup(void) {
  /* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
  delay(100);   
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY); // Using RX as GPIO

  //ESP.eraseConfig();
  PINCTRL_init(); 
  WIFIC_init();
  NTPS_init(); 
  WS_init();  
  HTTP_SERVER_init();  
}

void loop(void) { 
  HTTP_SERVER_process();
  WS_process();
  WIFIC_process();  
  NTPS_process();
}
