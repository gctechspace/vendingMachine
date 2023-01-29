#define dispenceTimeOut 10000
#define limitDebounce 50


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
    while (Serial1.available() > 0 && newData == false) {
        rc = Serial1.read();
        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            } else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                mqttClient.publish(DISPENCE_TOPIC, 0, 0, receivedChars);
                // numberOfMessages++;
                ndx = 0;
            }
        } else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void handleDispence()
{
  if(Serial1.available() > 0) {
    char rowChar = Serial1.read();
    Serial.print(rowChar);
//    if(inChar == 'M') { //Manually Add Credit to the machine
//      coinCredit+=(Serial1.parseInt()*10); 
//      return;
//    } else if(inChar == 'T') { //Manually dispense product
//       inChar = Serial1.read();
//    }
    if(rowChar >= 'A' && rowChar <= 'F'){
        byte traySelect = (1 << (5-(rowChar-65))); //Converts the Row (A-F) into a binary (1-32)
  
        int column = -1;
      
        char columnChar;
        unsigned long timeNow = millis();
        while(millis() - timeNow < 5000){
          if(Serial1.available()){
            columnChar = Serial1.read();
            if(columnChar >= '0' && columnChar <= '9'){
              Serial.println(columnChar);
              break;
            }
          }
        }
        if(columnChar < '0' || columnChar > '9'){
          return;
        }
        column = columnChar - '0';
        if(columnChar == '0') column = 10;
        int slotSelect = (1 << (10-column));
        int total = (traySelect << 10)|slotSelect;
        shiftDespence((total & 0xFF), (total>>8 & 0xFF), rowChar, columnChar);
    }
  }
}

void shiftDespence(byte itemCol, byte itemRow, char rowChar, char columnChar)
{
  boolean trayError = true;
  boolean dispenceError = true;
//  Serial.println("SHIFT");
//  Serial.print(itemCol); Serial.print(" "); Serial.println(itemRow);
//  Serial.print(itemCol, BIN); Serial.print(" "); Serial.println(itemRow, BIN);

  byte testdispenceLastState = digitalRead(despenceStatus);;
  byte testtrayLastState = digitalRead(trayStatus);

  digitalWrite(Shift_LATCH, LOW);
  shiftOut(Shift_DATA, Shift_CLOCK, LSBFIRST, itemCol);
  shiftOut(Shift_DATA, Shift_CLOCK, LSBFIRST, itemRow);
  digitalWrite(Shift_LATCH, HIGH);

  //wait for despence to finish then stop motors.
  unsigned long startTime = millis();
  while(millis() - startTime < dispenceTimeOut){
    byte testdispenceState = digitalRead(despenceStatus);
    if(testdispenceLastState != testdispenceState){
      if(testdispenceState == 1 && testdispenceLastState == 0) {
        delay(limitDebounce);
        dispenceError = false;
        break;
      }
      testdispenceLastState = testdispenceState;
    }   

    //test if Tray is connected
    byte testtrayState = digitalRead(trayStatus);
    if(testtrayLastState != testtrayState){
      if(testtrayState == LOW) {
        trayError = false;
      }
      testtrayLastState = testtrayState;
    }    
    delay(15);
    
  }
  clearShiftReg();
  Serial1.print("<");
  Serial1.print(rowChar);
  Serial1.print(columnChar);
  Serial1.print("|");
  if(trayError) {
    Serial.println("ERROR: Tray");
    Serial1.print("0");
//    Serial1.print(">");
//    return;
  } else if(dispenceError){
    Serial.println("ERROR: Dispence");
    Serial1.print('0');
  } else {
    Serial1.print('1');
  }
  Serial1.print(">");
}


void shiftRegInit()
{
  pinMode(Shift_DATA, OUTPUT);
  pinMode(Shift_ENABLE, OUTPUT);
  pinMode(Shift_LATCH, OUTPUT);
  pinMode(Shift_CLOCK, OUTPUT);
  digitalWrite(Shift_ENABLE, LOW);
  clearShiftReg();
}

//void convertSerial(char rowChar)
//{
//  byte traySelect = (1 << (5-(rowChar-65))); //Converts the Row (A-F) into a binary (1-32)
//  
//  int column = -1;
//
//  char columnChar;
//  unsigned long timeNow = millis();
//  while(millis() - timeNow < 5000){
//    if(Serial1.available()){
//      columnChar = Serial1.read();
//      if(columnChar >= '0' && columnChar <= '9'){
//        Serial.println(columnChar);
//        break;
//      }
//    }
//  }
//  if(columnChar >= '0' && columnChar <= '9'){
//  } else {
//    return;
//  }
//  column = columnChar - '0';
//  if(columnChar == '0') column = 10;
//  int slotSelect = (1 << (10-column));
//  int total = (traySelect << 10)|slotSelect;
////  shiftDespence((total & 0xFF), (total>>8 & 0xFF));
//}


void clearShiftReg(){
  digitalWrite(Shift_LATCH, LOW);
  shiftOut(Shift_DATA, Shift_CLOCK, LSBFIRST, 0);
  shiftOut(Shift_DATA, Shift_CLOCK, LSBFIRST, 0);
  shiftOut(Shift_DATA, Shift_CLOCK, LSBFIRST, 0);
  shiftOut(Shift_DATA, Shift_CLOCK, LSBFIRST, 0);
  digitalWrite(Shift_LATCH, HIGH);
}
