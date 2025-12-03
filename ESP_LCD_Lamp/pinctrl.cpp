/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  GPIO management module
 */
#include <Arduino.h>
#include "config.h"

static long lightOnTimestamp = 0;
static uint8_t lightState = 0;

void PINCTRL_init(){
  pinMode(LED_PIN, OUTPUT);
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
