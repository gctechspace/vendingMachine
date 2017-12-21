/* OLD CODE FOR TESTING MOTORS ETC - REFERENCE ONLY
const int enablePin = 13;
const int latchPin = 14;
const int clockPin = 15;
const int dataPin = 12;
const int IO1_Output = 11;
const int IO1_Status = 10;
const int IO2_Output = 26;
const int IO2_Status = 27;   //STATUS will go LOW when it has INPUT
const int IO3_Status = 1;
const int IO4_Status = 0;
const int IO5_Status = 23;
const int IO6_Status = 22;
const int IO7_Status = 21;
const int IO8_Status = 20;
const int IO9_Status = 19;
const int IO10_Status = 18;
const int watchDog = 16;

const int Selector = A0;  // Analog input pin that the voltage divider is attached to
const int selPwr = A1;    //Control of the NPN for the voltage dividers power

#define despenceTime 10000

void setup()
{
  //SETUP PORT INPUTS
  DDRC &= ~0xFC; //Set PC7-2 as OUTPUTS, FC=11111100  Doesn't affect the "0's"
  PORTC |= 0xFC; //Enable Pull Ups
  DDRB &= 0x03;  //set PB0-1 as OUTPUTS, 03=00000011
  PORTB |= 0x03;  //Enable Pull Ups
  DDRA &= 0xFC;
  //SETUP SHIFT REG
  //DDRD |= 0xF0;  //set PD4-7 as OUTPUTS, 0F=11110000
  //PORTD &= ~0x20; //set D13 (PD5) LOW
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);  //Enabled Low = ON, HIGH = OFF
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(IO1_Output, OUTPUT);  //D11
  pinMode(IO1_Status, INPUT);   //D10
  digitalWrite(IO1_Status, HIGH);     //Enable pull up
  pinMode(IO2_Output, OUTPUT);  //D26
  pinMode(IO2_Status, INPUT);   //D27
  digitalWrite(IO2_Status, HIGH);     //Enable pull up
  pinMode(watchDog, INPUT);  //SETUP PIN as watchDog


  Serial.begin(9600); //For Debugging & bluetooth
  Serial.println("Vending Test");



    pinMode(3, OUTPUT);

}

void loop()
{

  if(Serial.available() > 0) {
    byte traySelect = (1 << (Serial.read()-65)); //Converts the Row (A-F) into a binary (1-32)
    delay(100);
    byte slotSelect = processSlot(Serial.read());

    if(despence(traySelect, slotSelect)){
      Serial.println("Worked");
    } else {
      Serial.println("Error");
    }
  }
  //Serial.println(traySelect);
  //Serial.println(slotSelect);
}



byte processSlot(char inChar){
  if (inChar >= '1' && inChar <= '9'){
    return inChar-48;
  } else if (inChar == '0'){
    return 10;
  }
}

boolean despence(byte tray, byte slot){
  unsigned long despenceStart = 0;
  powerIO(tray);
  PORTA = (1 << (slot+1));  //the +1 is for testing as A2 is the first pin we are using on that port.
  despenceStart = millis();
  delay(100);
  //digitalWrite(A2, HIGH);
  while(digitalRead(IO10_Status)){
    if(millis() - despenceStart > despenceTime){
      powerIO(0);
      PORTA = 0x00; //All Off
      return false;
    }
  } //Get past the Low
  delay(200); //Debounce
  while(!digitalRead(IO10_Status)){
    if(millis() - despenceStart > despenceTime){
      powerIO(0);
      PORTA = 0x00; //All Off
      return false;
    }
  }  //Despence product
  delay(200); //Debounce
  powerIO(0);
  PORTA = 0x00; //All Off
  Serial.println(millis() - despenceStart);
  //digitalWrite(A2, LOW);
  return true;
}


//Turns Ports 3-10 ON/OFF
void powerIO(byte IOmap)  //pinout 3-10 int (1, 2, 4, 8, 16, 32, 64, 128)
{  //eg powerIO(16) turns IO7 ON, powerIO(34) turns 4 & 8 ON, powerIO(0) All OFF-ALWAYS DO THIS NOT JUST DISABLE
  digitalWrite(enablePin, LOW); //Turn Shift Reg ON
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, IOmap);
  digitalWrite(latchPin, HIGH);
}
*/
