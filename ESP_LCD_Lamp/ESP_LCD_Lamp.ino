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
#include "lcd_display.h"

static String statusMessage = "";         /* This is set and requested from other modules. */
static bool state_wifi_creds = false;
static bool state_show_clk_wait = false;


void MAIN_setStatusMsg(String msg){
  statusMessage = msg;
}

String MAIN_getStatusMsg(void){
  return statusMessage;
}

static void display_wifi_credentials()
{
  LCD_clear();
  LCD_textSize(2);
  LCD_color(C_YELLOW);
  LCD_write("WiFi SSID:\n");
  LCD_color(C_WHITE);
  // LCD_textSize(4);
  String message = String(WIFIC_getDeviceName()) + "\n";
  LCD_write(message);
  // LCD_textSize(3);
  LCD_color(C_YELLOW);
  LCD_write("WiFi PASS:\n");
  LCD_color(C_WHITE);
  // LCD_textSize(4);
  LCD_write(AP_PASS);
}

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
  LCD_init();
}

void loop(void) { 
  HTTP_SERVER_process();
  WS_process();
  NTPS_process();

  if(millis() > 20000){    
    if (NTPS_isReadyForDisplay()) {
        String hhmm = NTPS_getHHMM();
        LCD_clear();
        LCD_write(hhmm);  // display HH:MM
    } else if(!state_show_clk_wait){
      state_show_clk_wait = true;
      LCD_clear();
      LCD_write("Waiting for WiFi, NTP...");      
    }
  }else if(millis() > 7000){
    if(!state_wifi_creds){
      state_wifi_creds = true;
      display_wifi_credentials();
    }
  }
}
