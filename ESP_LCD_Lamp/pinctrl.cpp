/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  GPIO management module
 */
#include <Arduino.h>
#include "config.h"

static long lightOnTimestamp = 0;

void PINCTRL_init(){
  pinMode(LED_PIN, OUTPUT);
}

void PINCTRL_trigger(bool setOn)
{    
  if((millis() - lightOnTimestamp) < 500){
    return;
  }  

  digitalWrite(LED_PIN, setOn);
  lightOnTimestamp = millis();
}
