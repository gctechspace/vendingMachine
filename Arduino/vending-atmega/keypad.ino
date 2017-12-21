/*
#define selectTimeout 5000

void initKeypad(){
  ADCSRA = 0;  //Disable ADC

  //We power ON ONLY coloum 1
  pinMode(KP_Col1, OUTPUT);
  pinMode(KP_Col2, OUTPUT);
  pinMode(KP_Col3, OUTPUT);
  digitalWrite(KP_Col1, HIGH);
  digitalWrite(KP_Col2, LOW);
  digitalWrite(KP_Col3, LOW);
  DDRA = 0x00; //Port A, All Inputs.
}


void keyPressed(){
  byte key = 0;
  //byte rowCount = 0;
  byte rowShift = 0;
  byte colShift = 0;
  char row;
  byte col;

  key = manualKeyRead();
  if(key) {  //If we got a key Press
    blankScreen();  //****** TEMP Clear screen
    key -= 1; //Minus 1 to allow shifting
    digitalWrite(KP_Col1, LOW);
    row = 'A' + key;
    tft.print(row);
    Serial.print(row);

    rowShift = 1 << key;  //Shift
    //rowCount = 6-key;
    long time = millis();
    boolean rowSelected = false;
    while(!rowSelected){
      if(millis() - time > selectTimeout){
        Serial.println("Selection TimeOut");
        break;
      }
      digitalWrite(KP_Col2, HIGH);
      key = manualKeyRead();
      if(key) {
        if(key == 1){
          col = 1;
          rowShift += 64;
          rowSelected = true;
        } else {
          col = 1 + (key - 1)*2;
          Serial.println(col);
          colShift = 1 << (key - 2)*2;
          rowSelected = true;
        }
      }
      digitalWrite(KP_Col2, LOW);
      digitalWrite(KP_Col3, HIGH);
      key = manualKeyRead();
      if(key) {
        if(key == 1){
          col = 2;
          rowShift += 128;
          rowSelected = true;
        } else {
          col = 2 + (key - 1)*2;
          Serial.println(col);
          colShift = 1 << (((key - 1)*2)-1);
          rowSelected = true;
        }
      }
      digitalWrite(KP_Col3, LOW);

    }
    tft.println(col);
    //Serial.println("ColShift:10-1 / rowShift:F-A ");
    //Serial.print(colShift, BIN);  Serial.print(" "); Serial.println(rowShift, BIN);

    shiftDespence(colShift, rowShift); //Shift col out first then row. then Enable.
    blankScreen();  //****** TEMP Clear screen
    digitalWrite(KP_Col1, HIGH);
  }
}



byte manualKeyRead(){
  if(digitalRead(KP_RowA)) return 1;
  else if(digitalRead(KP_RowB)) return 2;
  else if(digitalRead(KP_RowC)) return 3;
  else if(digitalRead(KP_RowD)) return 4;
  else if(digitalRead(KP_RowE)) return 5;
  else if(digitalRead(KP_RowF)) return 6;
  else if(digitalRead(KP_RowG)) return 7;
  return 0;
}
*/
/***********************************OLD-NOT WORKING DIRECT MANIPULATION*****************************************
void getRow(){  //timer function or return slot
  byte row = 0;
  while(row == 0){
    digitalWrite(KP_Col2, HIGH);

    byte slot1Reading = PINA;  //READ Port A (the analog ports)
    delay(50);
    slot1Reading = PINA;
    if(slot1Reading != 0x00) {  //If one is HIGH
      Serial.println((getKey(slot1Reading, 1, 2))-1);  //data, Start place (Char or Number), Increment by.
      return;
    }
    digitalWrite(KP_Col2, LOW);
    delay(50);
    digitalWrite(KP_Col3, HIGH);

    byte slot2Reading = PINA;  //READ Port A (the analog ports)
    delay(50);
    slot2Reading = PINA;
    if(slot2Reading != 0x00) {  //If one is HIGH
      Serial.println((getKey(slot2Reading, 2, 2))-1);  //data, Start place (Char or Number), Increment by.
      return;
    }
    digitalWrite(KP_Col2, LOW);
    delay(50);
  }
}

byte getKey(byte data, byte range, byte increment){
  byte cnt = range - 1;
  for (byte mask = 10000000; mask > 0; mask >>= 1,cnt += increment) { //iterate through bit mask
    if (data & mask){ // if bitwise AND resolves to true
      return cnt;
    }
  }
}

*/
