/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  GPIO management module
 */
#include <Arduino.h>

#define LED_PIN   15
#define BTN_PIN   3

#define DEBOUNCE_MS 50

static long lightOnTimestamp = 0;
static uint8_t lightState = 0;
static int lastReading = HIGH;
static int ledIntensity = 100;

void PINCTRL_setIntesity(int i){
  ledIntensity = i * 10;  

  if(lightState == 1){
    analogWrite(LED_PIN, ledIntensity);
  }
}

int PINCTRL_getIntesity(){
  return ledIntensity/10;
}

void PINCTRL_init(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  analogWriteRange(100);
}

uint8_t PINCTRL_toggle()
{    
  if((millis() - lightOnTimestamp) < 500){
    return lightState;
  }  

  if(lightState == 0){
    lightState = 1;
    analogWrite(LED_PIN, ledIntensity);
  }else{
    lightState = 0;
    analogWrite(LED_PIN, 0);
  }
  
  lightOnTimestamp = millis();
  return lightState;
}

uint8_t PINCTRL_getCurrent()
{    
  return lightState; 
}

void PINCTRL_btnPressed() { 

  if (digitalRead(BTN_PIN) == LOW) {      
    PINCTRL_toggle();        
  }

  while(digitalRead(BTN_PIN) == LOW){
    delay(10);
  }
}

