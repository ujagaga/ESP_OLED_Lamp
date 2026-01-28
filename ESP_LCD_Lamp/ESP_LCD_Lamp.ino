#include "wifi_connection.h"
#include "config.h"
#include "pinctrl.h"
#include "http_server.h"
#include "NTPSync.h"
#include "web_socket.h"
#include "lcd_display.h"

enum Operation {
  Init,
  WifiCredentials,
  ConnectToAp,
  ShowIp,
  ShowTime
};

static Operation state = Init;
static uint32_t stateChangedAt = 0;
static int lastSeconds = 0;
static int blikX = 0;
static String statusMessage = "";         /* This is set and requested from other modules. */
static String timeStringHH = "";
static String timeStringMM = "";
static uint8_t oldLedState = 255;

void MAIN_setStatusMsg(String msg){
  statusMessage = msg;
}

String MAIN_getStatusMsg(void){
  return statusMessage;
}

static void display_wifi_credentials()
{
  LCD_clear();
  LCD_color(C_YELLOW);
  LCD_write("\nWiFi SSID:\n");
  LCD_color(C_WHITE);
  String message = String(WIFIC_getDeviceName());
  LCD_write(message);
  LCD_color(C_YELLOW);
  LCD_write("\nPASS:");
  LCD_color(C_WHITE);
  LCD_write(AP_PASS); 
  LCD_color(C_YELLOW);
  LCD_write("\nIP:");
  LCD_color(C_WHITE);
  LCD_write(WIFIC_getApIp());  
}

#ifdef USE_ADAFRUIT_ST7789
static void display_intro()
{
  LCD_clear();
  LCD_setFont(Font12pt);
  LCD_color(C_BLUE);
  LCD_write(" Secret Santa\n");
  LCD_color(C_GREEN);
  LCD_textSize(3);
  LCD_write("Clock");

  LCD_drawSnowflake(40, 180, 25, C_WHITE);
  LCD_drawSnowflake(90, 140, 25, C_WHITE);
  LCD_drawSnowflake(140, 170, 25, C_WHITE);
  LCD_drawSnowflake(190, 130, 25, C_WHITE);
}
#else
static void display_intro()
{  
  LCD_clear();   
  LCD_setFont(Font18pt);
  LCD_color(C_BLUE);
  LCD_write("\n  Ivanin sat");
  LCD_setFont(Font9pt);
}
#endif


void setup(void) 
{
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

void loop(void){
  HTTP_SERVER_process();
  WS_process();
  if(WIFIC_stationConnected()){
    NTPS_process();
  } 

  bool redraw = false;

  PINCTRL_btnPressed();
  uint8_t ledState = PINCTRL_getCurrent();
  if((ledState != 255) && (oldLedState != ledState)){
    oldLedState = ledState;
    LCD_setInverted(ledState);
    redraw = true;
    String bcmsg = "{\"CURRENT\":" + String(ledState) + "}";
    WS_ServerBroadcast(bcmsg);
  }

  // State machine
  switch(state){
    case Init:
    {
      display_intro();
      state = WifiCredentials;
      stateChangedAt = millis();
    }break;

    case WifiCredentials:
    {
      if((millis() - stateChangedAt) > 5000){
        display_wifi_credentials();
        state = ConnectToAp;
        stateChangedAt = millis();
      }      
    }break;

    case ConnectToAp:
    {
      if((millis() - stateChangedAt) > 5000){
        LCD_clear();
        LCD_write("\nWaiting for WiFi,\nNTP sync..."); 
        String stationIp = WIFIC_getStationIp();
        state = ShowIp;
        stateChangedAt = millis();        
      }   
    }break;

    case ShowIp:
    {
      String stationIp = WIFIC_getStationIp();
      if((stationIp.length() > 1)){
        LCD_color(C_YELLOW);
        LCD_write("\nConnected IP:\n");
        LCD_color(C_WHITE);
        LCD_write(stationIp);            
        state = ShowTime;
        stateChangedAt = millis();  
      }    
    }break;

    default:
    {
      if(NTPS_hasSynced()){
        String hh = NTPS_getHH();
        String mm = NTPS_getMM();

        if(!hh.equals(timeStringHH) || !mm.equals(timeStringMM) || redraw){
          timeStringHH = hh;
          timeStringMM = mm;

          LCD_clear(); 
          LCD_setFont(Font9pt);
          LCD_write("\n\n\n\n");  
          LCD_setFont(Font24pt);  
          LCD_textSize(2);
          LCD_write(timeStringHH);
          blikX = LCD_getX();
          LCD_write(":");
          LCD_write(timeStringMM);
        }

        int seconds = NTPS_getSeconds();
        if(lastSeconds != seconds){          
          lastSeconds = seconds;
          bool blinkOn = (lastSeconds % 2) == 0;
          LCD_setX(blikX);

          if(blinkOn){
            LCD_color(LCD_getFgColor());  
            LCD_write(":");
          }else{
            LCD_color(LCD_getBgColor());  
            LCD_write(":");
          }
        }
      }
    }break;
  }

}

