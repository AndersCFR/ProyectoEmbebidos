#include <Arduino.h>

int potenciometroR = A0;
int potenciometroG = A1;
int potenciometroB = A2;
int luzR = 9;
int luzG = 10;
int luzB = 11;
int brilloR;
int brilloG;
int brilloB;


void setup() {
  //Salidas PWM
  pinMode(9, OUTPUT);

  //pinMode(potenciometroR, INPUT);
  Serial.begin(9600);
}

void loop() {

  brilloR = map(analogRead(potenciometroR),0,1023,0,255);
  brilloG = map(analogRead(potenciometroG),0,1023,0,255);
  brilloB = map(analogRead(potenciometroB),0,1023,0,255);
  analogWrite(luzR, brilloR);
  analogWrite(luzG, brilloG);
  analogWrite(luzB, brilloB);
}