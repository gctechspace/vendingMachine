/* NEXTION CODES
 *  H = Home Page
 *    home.creditText.txt
 *    
 *  A-F = ROW
 *  1-0 = Column (1 to 10)
 *  
 *  S = Sensor Page  /Same as below
 *  I = Info Page
 *    info.itemNameText.txt
 *    info.infoText.txt
 *    info.priceText.txt
 *  
 *  M = Add Credit
 *    Money Value comes next
 *  P = Set Price
 *    Row/Column/Money Value
 *  T = Test Motor
 *    Row/Cloumn
 */
//write2Nextion("home.slider.val=", mb.Hreg(TEMP_SETPOINT)/10);
 
//void handleNextion()
//{
//  if(Serial1.available() > 0) {
//    char inChar = Serial1.read();
//    Serial.print(inChar);
//    if(inChar == 'M') { //Manually Add Credit to the machine
//      coinCredit+=(Serial1.parseInt()*10); 
//      return;
//    } else if(inChar == 'T') { //Manually dispense product
//       inChar = Serial1.read();
//    }
//    if(inChar >= 'A' && inChar <= 'F'){
//      convertSerial(inChar);
//    }
//    //for TESTING
//    if(inChar == 'M'){
//      //change page on Nextion
//    }
//  }
// }

// void writeCredit2Nextion(String targetField, int value)
//{
//  int dollar = value/100;
//  int cents = value % 100;
//  Serial1.print(targetField);
//  Serial1.write(0x22);
//  Serial1.print(dollar);
//  Serial1.print(".");
//  Serial1.print(cents);
//  if(cents == 0) {
//    Serial1.print(cents);
//  }
//  Serial1.write(0x22);
//  commandTail();
//}

void writePercent2Nextion(String targetField, int value)
{
  Serial1.print(targetField);
  Serial1.write(0x22);
  Serial1.print(value);
  Serial1.print("%");
  Serial1.write(0x22);
  commandTail();
}

void write2Nextion(String targetField, int value)
{
  Serial1.print(targetField);
  Serial1.write(0x22);
  Serial1.print(value);
  Serial1.write(0x22);
  commandTail();
}

void commandTail()
{
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}
