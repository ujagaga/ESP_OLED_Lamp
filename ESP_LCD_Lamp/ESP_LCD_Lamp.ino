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
static String timeStringHH = "";
static String timeStringMM = "";
static bool stationIpDisplayed = false;

void MAIN_setStatusMsg(String msg){
  statusMessage = msg;
}

String MAIN_getStatusMsg(void){
  return statusMessage;
}

static void display_wifi_credentials()
{
  LCD_clear();
  LCD_textSize(3);
  LCD_color(C_YELLOW);
  LCD_write("WiFi SSID:\n");
  LCD_color(C_WHITE);
  String message = String(WIFIC_getDeviceName()) + "\n";
  LCD_write(message);
  LCD_color(C_YELLOW);
  LCD_write("WiFi PASS:\n");
  LCD_color(C_WHITE);
  LCD_write(AP_PASS); 
}

void setup(void) {
  /* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
  delay(100);   
  Serial.begin(115200); 
  PINCTRL_init(); 
  WIFIC_init();
  WS_init();  
  HTTP_SERVER_init();  
  LCD_init();
  NTPS_init();
}

void loop(void) { 
  HTTP_SERVER_process();
  WS_process();
  NTPS_process();

  if(millis() > 20000){    
    if(!state_show_clk_wait)
    {
      state_show_clk_wait = true;
      LCD_clear();
      LCD_write("Waiting for WiFi,\nNTP sync...");            
    }else {
      String hh = NTPS_getHH();
      String mm = NTPS_getMM();

      if(!hh.equals(timeStringHH) || !mm.equals(timeStringMM)){
        timeStringHH = hh;
        timeStringMM = mm;

        LCD_clear();
        LCD_color(C_YELLOW);
        LCD_textSize(8);        
        LCD_write(" ");
        LCD_textSize(14);
        LCD_write(hh);  
        LCD_write("\n");
        LCD_textSize(8);        
        LCD_write(" ");
        LCD_textSize(14);
        LCD_write(mm);
      }
    }  
  }else if(millis() > 7000){
    if(!state_wifi_creds){
      state_wifi_creds = true;
      display_wifi_credentials();
    }
    String stationIp = WIFIC_getStationIp();
    if((stationIp.length() > 1) && !stationIpDisplayed){
      LCD_color(C_YELLOW);
      LCD_write("\nConnected IP:\n");
      LCD_color(C_WHITE);
      LCD_write(stationIp);
      stationIpDisplayed = true;      
    }
  }

  uint8_t ledState = PINCTRL_btnPressed();
  if(ledState != 255){
    String bcmsg = "{\"CURRENT\":" + String(ledState) + "}";
    WS_ServerBroadcast(bcmsg);
  }
}
