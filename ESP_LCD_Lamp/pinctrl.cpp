/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  GPIO management module
 */
#include <Arduino.h>
#include "config.h"

#define DEBOUNCE_MS 50

static long lightOnTimestamp = 0;
static uint8_t lightState = 0;
static int lastReading = HIGH;

void PINCTRL_init(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
}

uint8_t PINCTRL_toggle()
{    
  if((millis() - lightOnTimestamp) < 500){
    return lightState;
  }  

  if(lightState == 0){
    lightState = 1;
    digitalWrite(LED_PIN, HIGH);
  }else{
    lightState = 0;
    digitalWrite(LED_PIN, LOW);
  }
  
  lightOnTimestamp = millis();
  return lightState;
}

uint8_t PINCTRL_getCurrent()
{    
  return lightState; 
}

uint8_t PINCTRL_btnPressed() { 

  int retVal = 255;
  if (digitalRead(BTN_PIN) == LOW) {      
    retVal = PINCTRL_toggle();        
  }

  while(digitalRead(BTN_PIN) == LOW){
    delay(10);
  }
  return retVal;
}

