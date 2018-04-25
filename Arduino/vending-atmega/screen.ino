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
 
void handleNextion()
{
  if(Serial1.available() > 0) {
    char inChar = Serial1.read();
    Serial.print(inChar);
    if(inChar == 'M') { //Manually Add Credit to the machine
      coinCredit+=(Serial1.parseInt()*10); 
      return;
    } else if(inChar == 'T') { //Manually dispense product
       inChar = Serial1.read();
    }
    if(inChar >= 'A' && inChar <= 'F'){
      convertSerial(inChar);
    }
    //for TESTING
    if(inChar == 'M'){
      //change page on Nextion
    }
  }
 }
