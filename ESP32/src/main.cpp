#include <Arduino.h>

#define BEEP_PIN 16
#define ADC_PIN 36

void setup() {
  Serial.begin(9600);
  pinMode(BEEP_PIN, OUTPUT);
  pinMode(ADC_PIN, INPUT);
  ledcSetup(0,2000,8);
  ledcAttachPin(BEEP_PIN,0);
}
void loop() {
  ledcWriteTone(0,2000);
  // uint16_t sensorValue=analogRead(ADC_PIN);
  // u_int8_t value1=sensorValue*(5.0/1023.0);
  // Serial.println(value1);
  // delay(1000);
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle = dutyCycle + 10) {
    Serial.println(dutyCycle);
    ledcWrite(0, dutyCycle);
    delay(1000);
  }
  
  ledcWrite(0, 125);
  for (int freq = 255; freq < 10000; freq = freq + 250) {
    Serial.println(freq);
    ledcWriteTone(0, freq);
    delay(1000);
  }
}

         