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
  Serial.println("Project by SkipXtian");
  Serial.println("MickMorri,JamesSapsford,Spidie");

  attachInterrupt(coinIn, coinInserted, RISING);   //If coinInt goes HIGH (a Pulse), call the coinInserted function

  pinMode(despenceStatus, INPUT_PULLUP);
  pinMode(trayStatus, INPUT_PULLUP);

  //rfidInit();
  shiftRegInit();
}

void loop()
{
  handleNextion();
  
  //detectTag();

  // testing
  // if(coinCredit != 0) {
  //   Serial.println(coinCredit);
  //   coinCredit = 0;
  // }
}

//byte processSlot(char inChar){
//  if (inChar >= '1' && inChar <= '9'){
//    return inChar-48;
//  } else if (inChar == '0'){
//    return 10;
//  }
//}

void coinInserted()
{
//The function that is called every time it receives a pulse
  coinCredit += 50;
}  //The coin selector is set to pulse once for every 50cents
