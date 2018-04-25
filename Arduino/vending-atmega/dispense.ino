#define dispenceTimeOut 10000
#define limitDebounce 50


void shiftDespence(byte itemCol, byte itemRow)
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

  if(trayError) {
    Serial.println("ERROR: Tray");
  } else if(dispenceError){
    Serial.println("ERROR: Dispence");
  } 
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

void convertSerial(char rowChar)
{
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
  if(columnChar >= '0' && columnChar <= '9'){
  } else {
    return;
  }
  column = columnChar - '0';
  if(columnChar == '0') column = 10;
  int slotSelect = (1 << (10-column));
  int total = (traySelect << 10)|slotSelect;
  shiftDespence((total & 0xFF), (total>>8 & 0xFF));
}


void clearShiftReg(){
  digitalWrite(Shift_LATCH, LOW);
  shiftOut(Shift_DATA, Shift_CLOCK, LSBFIRST, 0);
  shiftOut(Shift_DATA, Shift_CLOCK, LSBFIRST, 0);
  shiftOut(Shift_DATA, Shift_CLOCK, LSBFIRST, 0);
  shiftOut(Shift_DATA, Shift_CLOCK, LSBFIRST, 0);
  digitalWrite(Shift_LATCH, HIGH);
}
