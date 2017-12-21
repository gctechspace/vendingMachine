#define timeOut 10000
#define debounce 1000

void shiftRegInit(){
  pinMode(Shift_DATA, OUTPUT);
  pinMode(Shift_ENABLE, OUTPUT);
  pinMode(Shift_LATCH, OUTPUT);
  pinMode(Shift_CLOCK, OUTPUT);
  digitalWrite(Shift_ENABLE, LOW);
  clearShiftReg();
}

void shiftDespence(byte itemCol, byte itemRow)
{
  //Serial.println("SHIFT");
  //Serial.print(itemCol); Serial.print(" "); Serial.println(itemRow);
  //Serial.print(itemCol, BIN); Serial.print(" "); Serial.println(itemRow, BIN);
  
  digitalWrite(Shift_LATCH, LOW);
  shiftOut(Shift_DATA, Shift_CLOCK, MSBFIRST, itemCol);
  shiftOut(Shift_DATA, Shift_CLOCK, MSBFIRST, itemRow);
  digitalWrite(Shift_LATCH, HIGH);
  
  //wait for despence to finish then stop motors.
  long startTime = millis();
  while(digitalRead(despenceStatus) && millis() - startTime < timeOut){} //While we are over the low part do nothing
  delay(debounce);
  while(!digitalRead(despenceStatus) && millis() - startTime < timeOut){} //While we are over the high part do nothing

  clearShiftReg();
}


void clearShiftReg(){
  digitalWrite(Shift_LATCH, LOW);
  shiftOut(Shift_DATA, Shift_CLOCK, MSBFIRST, 0);
  shiftOut(Shift_DATA, Shift_CLOCK, MSBFIRST, 0);
  digitalWrite(Shift_LATCH, HIGH);
}
