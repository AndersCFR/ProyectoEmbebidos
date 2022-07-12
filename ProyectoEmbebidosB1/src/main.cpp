#include <Arduino.h>
#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <LCD.h>
#include <EEPROM.h>
#include <stdio.h>
#include<string.h>

#define NOTE_C2  65
#define NOTE_G2  98
#define NOTE_C5  523

// Prototipo funciones
void luzInterna();
void luzExterna();
void sonido(int numeroMelodia);
void Alarma();
void pantalla();

// Configuración teclado
const byte FILAS = 1;
const byte COLUMNAS = 2;
char keys[FILAS][COLUMNAS] = {
  {'1','2'}
};
byte pinesFilas[FILAS] = {13};
byte pinesColumnas[COLUMNAS] = {12,10};

Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);

char TeclaPresionada;
char CLAVE[5];        // almacena en un array 6 digitos ingresados
byte INDICE = 0;      // indice del array

//char CLAVE_MAESTRA[5] = "0000";   // almacena en un array la contraseña maestra

// Luces internas
int potenciometroR = A0;
int potenciometroG = A1;
int potenciometroB = A2;
int luzR = 5;
int luzG = 6;
int luzB = 11;
int brilloR;
int brilloG;
int brilloB;

// Luces Externas
int oscuridad;
int sensorLDR = A3;
int luzAmarilla = 8;

// Buzzer
int buzzer = 7;

// Servomotor
Servo servo; 

// lcd
LiquidCrystal_I2C lcd (0X27, 2, 1, 0, 4, 5, 6, 7);

// PIR
int PIR = 2;
int EstadoAlarma = 0;

// Botón
int boton = 4;
int inicio = 0;
int AlarmaEncendidaAC = 0;
int AlarmaEncendidaAN = 0;

// Obtención de clave de la posición 10 de la memoria
char clave[] = "    ";

void setup() {
  //EEPROM, memoria
  Serial.print(EEPROM.get(10,clave));
  //Salidas Digitales
  pinMode(luzAmarilla, OUTPUT);
  pinMode(buzzer, OUTPUT);
  //Comunicación
  servo.attach(9);
  Serial.begin(9600);
  // lcd - i2c
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16,2);
  lcd.clear();
  // PIR
  pinMode(PIR, INPUT);
  // Botón
  pinMode(boton, INPUT);
}

void loop() {
  luzInterna();
  luzExterna();
  pantalla();
  TeclaPresionada = teclado.getKey();
  if (TeclaPresionada) {
    CLAVE[INDICE] = TeclaPresionada;
    INDICE++;      
    Serial.println(TeclaPresionada);
    lcd.print(TeclaPresionada);
    sonido(1);
  }
  
  if(INDICE == 4){
    if(!strcmp(CLAVE, clave)){   // compara clave ingresada con clave maestra
      Serial.println(" Correcta");  // imprime en monitor serial que es correcta la clave
      lcd.setCursor(0,3);
      lcd.print("Correcto!   ");
      servo.write(120);
      delay(2000);
      servo.write(0);
      }
    else{
      Serial.println(" Incorrecta");  // imprime en monitor serial que es incorrecta la clave      
      lcd.setCursor(0,3);
      lcd.print("Incorrecto!");
      sonido(0);
    }     
    INDICE = 0;
  } 

  Alarma();
  
}


// Funciones para usar

void luzInterna(){
  brilloR = map(analogRead(potenciometroR),0,1023,0,255);
  brilloG = map(analogRead(potenciometroG),0,1023,0,255);
  brilloB = map(analogRead(potenciometroB),0,1023,0,255);
  analogWrite(luzR, brilloR);
  analogWrite(luzG, brilloG);
  analogWrite(luzB, brilloB);
  delay(200);
}

void luzExterna(){
  oscuridad = analogRead(sensorLDR);
  if (oscuridad <= 80 && oscuridad > 0){
    digitalWrite(luzAmarilla,HIGH);    
  }else{
    digitalWrite(luzAmarilla,LOW);    
  }  
}

void sonido(int melodia){
  int melodiaIncorrecto[] = {NOTE_C2, NOTE_G2};
  int duracionesIncorrecto[] = {4,4,4};
  int melodiaCorrecto[] = {NOTE_C5};
  int duracionesCorrecto[] = {4};


  if(melodia == 0){
     for (int i = 0; i < 2; i++) {			// bucle repite 8 veces
        int duracion = 1000 / duracionesIncorrecto[i];		// duracion de la nota en milisegundos
        tone(buzzer, melodiaIncorrecto[i], duracion);	// ejecuta el tono con la duracion
        int pausa = duracion * 1.30;			// calcula pausa
        delay(pausa);					// demora con valor de pausa
        noTone(buzzer);		// detiene reproduccion de tono
      } 		
  }else if(melodia == 1) {
    for (int i = 0; i < 1; i++) {			// bucle repite 8 veces
        int duracion = 1000 / duracionesCorrecto[i];		// duracion de la nota en milisegundos
        tone(buzzer, melodiaCorrecto[i], duracion);	// ejecuta el tono con la duracion
        int pausa = duracion * 1.30;			// calcula pausa
        delay(pausa);					// demora con valor de pausa
        noTone(buzzer);		// detiene reproduccion de tono
      } 		
  }
  
}


void Alarma(){
  AlarmaEncendidaAC = digitalRead(boton);
  if (AlarmaEncendidaAC && AlarmaEncendidaAN == 0){
    inicio = 1 - inicio;
    delay(100);
  }
  AlarmaEncendidaAC = AlarmaEncendidaAN;

  if (inicio == 1){
  EstadoAlarma = digitalRead(PIR);
    if (EstadoAlarma == HIGH){
      // Sonar alarma
      sonido(1);
      // Parpadear luces externas
      digitalWrite(luzAmarilla,HIGH);  
      // Parpadear luces internas
      analogWrite(luzR, 255);
      analogWrite(luzG, 255);
      analogWrite(luzB, 255);      
      // Avisar en pantalla
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("  Alerta!   ");
      delay(50);
      
  }
  } 
}

void pantalla(){
  Serial.println(clave);
  lcd.setCursor(0,0);
  lcd.print("Alar: ");
  lcd.print(inicio);
  lcd.print(" Clave:");
}