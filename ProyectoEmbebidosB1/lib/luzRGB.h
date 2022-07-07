#define LuzRGB_H
#include <Arduino.h>
#include <iostream>

using namespace std;

class LuzRGB {

private:
    int luzR;
    int luzG;
    int luzB;
    int entradaR;
    int entradaG;
    int entradaB;
    int brilloR;
    int brilloG;
    int brilloB;

public:
    LuzRGB(int pinR, int pinG, int pinB, int valorR, int valorG, int valorB)
        : luzR(pinR), luzG(pinG), luzB(pinB), entradaR(valorR), entradaG(valorG), entradaB(valorB)
        {
            Serial.begin(9600);
            pinMode(pinR, OUTPUT);
        }

public:
    void luzAjustable();

void luzAjustable(){
    brilloR = map(analogRead(entradaR),0 ,1023, 0, 255);
    cout<<analogRead(entradaR);

    digitalWrite(luzR, brilloR);
}

};
