#define ver 001

#include "pinDef_v0_1.h"

volatile unsigned int coinCredit = 0;  //Holds the current inserted coins credit

//***************Libraries***********************
#include "SPI.h"
//QVGA Screen
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"
//RFID
#include <MFRC522.h>
//SDCard
#include <SD.h>


//***************Instances**************************
Adafruit_ILI9340 tft = Adafruit_ILI9340(QVGA_CS, QVGA_DC, QVGA_RST);  
MFRC522 mfrc522(RFID_CS, RFID_RST);        
File myFile; 


void setup(){
  Serial.begin(9600);
  Serial.print("Vending Machine, Version:");
  Serial.println(ver);
  Serial.println("Designed by Skip Christian");
  
  attachInterrupt(coinIn, coinInserted, RISING);   //If coinInt goes HIGH (a Pulse), call the coinInserted function
  
  digitalWrite(despenceStatus, HIGH);
  digitalWrite(trayStatus, HIGH);
  
  initKeypad();
  screenInit();
  rfidInit();
  shiftRegInit();
}

void loop(){
  keyPressed();
  detectTag();
  if(coinCredit != 0) {
    Serial.println(coinCredit);
    coinCredit = 0;
  }
}

void coinInserted(){
//The function that is called every time it receives a pulse
  coinCredit += 50;  
}  //The coin selector is set to pulse once for every 50cents

