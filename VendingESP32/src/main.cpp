#include <Arduino.h>

#include "SPI.h"
#include "MFRC522.h"

#include "WiFi.h"
// #include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoOTA.h>
extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include <AsyncMqttClient.h>
#include <HardwareSerial.h>
#include "credentials.h"
#include "FS.h"
#include "SPIFFS.h" //A way to store configuration with some memory protection

//****************** FUNCTION PROTOTYPES ******************
void initOTA(void);
void write2Nextion(String targetField, String value);
void write2Nextion(String targetField, unsigned long value);
byte getItemIndex(char row, char column);

// HardwareSerial dispenceSerial(1); // Pins 16,17
HardwareSerial dispenceSerial(2); // Pins 4,15

bool attemptDispence = false;
unsigned long attemptDispenceTime;
bool maintance = false; //won't publish to broker a dispence if in maintaince mode (does not effect stock count)

#define COIN_PULSE_PIN 27 //Right on the end of the serial connection to the Nextion
#define NO_ITEM_SELECTED '\0'
// byte itemArrayIndex = 0;

#define ITEM_COUNT 39

typedef struct
{
	// char fileName[3];  //itemArrayIndex in a char array
	char itemName[15];
	uint16_t price; //Divide by 100 ie 350 = 3.5
	byte stockLeft;
	// byte stockReserved;
	char description[25];
} stockStruct;

stockStruct item[ITEM_COUNT];

typedef struct
{
	char itemRow;
	char itemColumn;
	uint16_t price;
	// unsigned long timeSelected;
} itemStruct;

itemStruct selectedItem;

//item[itemArrayIndex].name = "Test";

#define DOOR_OPEN_PIN 14 //WS2812 Pinout

//****************** CONNECTION HANDLING ******************
WiFiClient espClient;
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

//****************** TIME HANDLING ******************
WiFiUDP ntpUDP;
// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
#define TIMEZONE 10 //Australia Brisbane GMT+10
NTPClient timeClient(ntpUDP, "au.pool.ntp.org", 3600 * TIMEZONE, 60000);

//**********************Coin Mech**********************
volatile unsigned int coinCredit = 0; //Holds the current inserted coins credit
volatile unsigned long coinInsertedTime = 0;
unsigned int moneyInserted = 0;
#define coinPulseDelay 150

void coinInsertedISR()
{
	//The function that is called every time it receives a pulse
	static unsigned long lastPulse = 0;
	if (millis() - lastPulse > 80)
	{
		lastPulse = millis();
		coinInsertedTime = lastPulse;
		coinCredit++;
	}
} //The coin selector is set to pulse once for every 50cents

//****************** CODE SEPERATION ******************
#include "fileSystem.esp"
#include "nextion.esp"
#include "connection.esp"
#include "rfid.esp"

/******************** Gives the pulses from the coin mech a value ******************/
void handleCoins()
{
	if (coinCredit != 0 && millis() - coinInsertedTime > coinPulseDelay)
	{
		// Serial.println(coinCredit);
		// Serial.println(moneyInserted);
		moneyInserted += coinCredit * 50;
		Serial.println(moneyInserted);
		Serial.println();
		coinCredit = 0;
		writePrice2Nextion("home.creditText.txt=", moneyInserted);
		char moneystr[6];
		itoa(moneyInserted, moneystr, 10);
		mqttClient.publish(MONEY_TOPIC, 1, 0, moneystr);
	}
}

void handleSelectedItem()
{
	if (selectedItem.itemRow != NO_ITEM_SELECTED && attemptDispence == false)
	{
		if (moneyInserted >= selectedItem.price)
		{
			logEvent("[LOG]: Atempting Dispecnce");
			attemptDispence = true;
			attemptDispenceTime = millis();
			dispenceSerial.print('<');
			dispenceSerial.print(selectedItem.itemRow);
			dispenceSerial.print(selectedItem.itemColumn);
			dispenceSerial.print(selectedItem.itemRow + selectedItem.itemColumn);
			dispenceSerial.print('>');
		}
	}
}

const byte numChars = 10;
char receivedChars[numChars];
boolean newData = false;
void recvWithStartEndMarkers()
{
	static boolean recvInProgress = false;
	static byte ndx = 0;
	char startMarker = '<';
	char endMarker = '>';
	char rc;
	unsigned long startTime = 0;

	while (dispenceSerial.available() > 0 && newData == false)
	{
		rc = dispenceSerial.read();

		if (recvInProgress == true)
		{
			if (rc != endMarker)
			{
				receivedChars[ndx] = rc;
				ndx++;
				if (ndx >= numChars)
				{
					ndx = numChars - 1;
				}
			}
			else
			{
				receivedChars[ndx] = '\0'; // terminate the string
				recvInProgress = false;
				// mqttClient.publish(DISPENCE_TOPIC, 0, 0, receivedChars);
				// numberOfMessages++;
				if (receivedChars[3] == '1') // SWAPPED 24/11/21
				{
					moneyInserted -= selectedItem.price;
					writePrice2Nextion("home.creditText.txt=", moneyInserted);
					item[getItemIndex(selectedItem.itemRow, selectedItem.itemColumn)].stockLeft -= 1;
					selectedItem.price = 0;
					selectedItem.itemRow = NO_ITEM_SELECTED;
				}
				else if (receivedChars[3] == '0') // SWAPPED 24/11/21
				{
					logEvent("[ERR]: Tray not found");
				}
				else if (receivedChars[3] == '2')
				{
					logEvent("[ERR]: Dispence Timeout");
				}
				else if (receivedChars[3] == '3')
				{
					logEvent("[ERR]: Bad Checksum");
				}
				mqttClient.publish(DISPENCE_TOPIC, 0, 0, receivedChars);
				attemptDispence = false;
				logEvent("[LOG]: End dispence");

				ndx = 0;
			}
		}
		else if (rc == startMarker)
		{
			recvInProgress = true;
			startTime = millis();
		}
	}
	if (recvInProgress == true && millis() - startTime > 3000)
	{
		recvInProgress = false;
		logEvent("[ERR]: Serial timeout");
	}
	if (attemptDispence == true && millis() - attemptDispenceTime > 15000)
	{
		attemptDispence = false;
		logEvent("[ERR]: Dispence Timed out");
		// writePrice2Nextion("home.creditText.txt=", "Error");
		char dispenced[5];
		dispenced[0] = selectedItem.itemRow;
		dispenced[1] = selectedItem.itemColumn;
		dispenced[2] = '|';
		dispenced[3] = '4';
		dispenced[4] = '\0';
		mqttClient.publish(DISPENCE_TOPIC, 1, 0, dispenced);
		selectedItem.price = 0;
		selectedItem.itemRow = NO_ITEM_SELECTED;
	}
}

void handleDispence()
{
	recvWithStartEndMarkers();
	// char dispenced[5];
	// if (dispenceSerial.available() > 0)
	// { //expecting "<A1|1>" row&column|sucess/failure
	// 	for (byte i = 0; i < 4; i++)
	// 	{
	// 		dispenced[i] = dispenceSerial.read();
	// 	}
	// 	if (maintance == false)
	// 	{
	// 		dispenced[4] = '\0';
	// 		if (dispenced[3] == '0')
	// 		{
	// 			moneyInserted -= selectedItem.price;
	// 			writePrice2Nextion("home.creditText.txt=", moneyInserted);
	// 			item[getItemIndex(selectedItem.itemRow, selectedItem.itemColumn)].stockLeft -= 1;
	// 			selectedItem.price = 0;
	// 			selectedItem.itemRow = NO_ITEM_SELECTED;
	// 		}
	// 		else if (dispenced[3] == '1')
	// 		{
	// 			logEvent("[ERR]: Tray not found");
	// 		}
	// 		else if (dispenced[3] == '2')
	// 		{
	// 			logEvent("[ERR]: Dispence Timeout");
	// 		}
	// 		else if (dispenced[3] == '3')
	// 		{
	// 			logEvent("[ERR]: Bad Checksum");
	// 		}
	// 		mqttClient.publish(DISPENCE_TOPIC, 1, 0, dispenced);
	// 		logEvent("[ERR]: What the?");
	// 	}
	// 	else
	// 	{
	// 		maintance = false;
	// 	}
	// 	attemptDispence = false;
	// 	logEvent("[ERR]: What the?");
	// }

	// if (attemptDispence == true && millis() - attemptDispenceTime > 15000)
	// {
	// 	attemptDispence = false;
	// 	logEvent("[ERR]: Dispence Timed out");
	// 	// writePrice2Nextion("home.creditText.txt=", "Error");
	// 	dispenced[0] = selectedItem.itemRow;
	// 	dispenced[1] = selectedItem.itemColumn;
	// 	dispenced[2] = '|';
	// 	dispenced[3] = '4';
	// 	dispenced[4] = '\0';
	// 	mqttClient.publish(DISPENCE_TOPIC, 1, 0, dispenced);
	// 	selectedItem.price = 0;
	// 	selectedItem.itemRow = NO_ITEM_SELECTED;
	// }
}

void doorStateChange()
{
	static boolean lastDoorState = digitalRead(DOOR_OPEN_PIN);
	boolean doorState = digitalRead(DOOR_OPEN_PIN);
	if (lastDoorState != doorState)
	{
		if (doorState == HIGH)
		{ //Door closed return to normal display
			logEvent("[INFO]: Door Closed");
			write2Nextion("page home");
		}
		else
		{ //Door opened show maintaince screen
			logEvent("[INFO]: Door Opened");
			write2Nextion("page maintance");
		}
		lastDoorState = doorState;
	}
}

//****** Returns the array index for an item location ******
byte getItemIndex(char row, char column)
{
	byte itemArrayIndex = 0;
	if (row == 'B')
		itemArrayIndex = 5;
	else if (row == 'C')
		itemArrayIndex = 10;
	else if (row == 'D')
		itemArrayIndex = 15;
	else if (row == 'E')
		itemArrayIndex = 25;
	else if (row == 'F')
		itemArrayIndex = 35;
	if (column == '0')
	{
		itemArrayIndex += column - 39;
	}
	else
	{
		itemArrayIndex += column - 49;
	}
	return itemArrayIndex;
}

//******************************* SETUP **********************************
void setup()
{

#ifndef DEBUG
	Serial.begin(115200);
	Serial.println("");
	Serial.println("Sketch init!");
	Serial.print("Compile Date / Time : ");
	Serial.print(__DATE__);
	Serial.print(" / ");
	Serial.println(__TIME__);
	Serial.println("");
#endif
	dispenceSerial.begin(9600, SERIAL_8N1, 25, 14); //4, 15);
													//nRF plug  x,x,Tx,GND
	// x,x,Rx,x

	//  Bolt GND,25,12    OLED GND,3v3,22,21
	// dispenceSerial.begin(9600, SERIAL_8N1, 16, 17);

	initNextion();

	initRFID();

	pinMode(DOOR_OPEN_PIN, INPUT_PULLUP);
	attachInterrupt(COIN_PULSE_PIN, coinInsertedISR, RISING); //If coinInt goes HIGH (a Pulse), call the coinInserted function
	pinMode(DOOR_OPEN_PIN, INPUT_PULLUP);
	// attachInterrupt(DOOR_OPEN_PIN, doorStateChangeISR, CHANGE);

	initWifi();
	initSpiffs();
	enableWDT(); //This is the core 1 watchdog timer, it's feed in houseKeeping.
}

//******************************* LOOP **********************************
void loop()
{
	houseKeeping(); //Wifi, MQTT etc

	doorStateChange(); //Enter Maintaince Mode
	handleNextion();   //Commands from touch screen
	handleCoins();
	handleSelectedItem();
	handleRFID();
	handleDispence();
}
