#define ver 002
/*
Possibilities:
Add ESP32/8266 and connect:
Coin Mech (through voltage divider)
Nextion Display (Serial)
VendingBoard (I2C)
*TFT* <= QR Codes or something similar

Leaving VendingBoard to Control Dispence
*/

#include "pinDef_v0_1.h"

volatile unsigned int coinCredit = 0;  //Holds the current inserted coins credit
volatile unsigned long coinInsertedTime = 0;
unsigned int moneyInserted = 0;
#define coinPulseDelay 150

//***************Libraries***********************

//RFID
#include <MFRC522.h>

//***************Instances**************************
MFRC522 mfrc522(RFID_CS, RFID_RST);



void setup()
{
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial.print("Vending Machine, Version:");
  Serial.println(ver);
  Serial.println("Project by SkipXtian, DTBaker");

//  attachInterrupt(coinIn, coinInsertedISR, RISING);   //If coinInt goes HIGH (a Pulse), call the coinInserted function

  pinMode(despenceStatus, INPUT_PULLUP);
  pinMode(trayStatus, INPUT_PULLUP);

  //rfidInit();
  shiftRegInit();
}

void loop()
{
//  handleNextion();
  handleDispence();
  //detectTag();


//   if(coinCredit != 0 && millis()-coinInsertedTime>coinPulseDelay) {
//     Serial.println(coinCredit);
//     moneyInserted += coinCredit;
//     writeCredit2Nextion("home.creditText.txt=", moneyInserted);
//     coinCredit = 0;
//   }
}



void coinInsertedISR()
{
//The function that is called every time it receives a pulse
  coinInsertedTime = millis();
  coinCredit += 50;
}  //The coin selector is set to pulse once for every 50cents
