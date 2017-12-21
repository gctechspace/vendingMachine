void initSDCard(){
  pinMode(4, OUTPUT);  // <<Can be changed
  if(!SD.begin(SDCard_CS)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void testLineRead(){
 if(Serial.available()){
   byte inNumber = Serial.read() - 48;
   lineData(inNumber);
 }
}

void lineData(byte line){
    // Open the file for reading:
  myFile = SD.open("stock.txt");
  if (myFile) {

    myFile.seek(line * 22);  //Seek the line (each line is 22 bytes long (20 + \n)
    for(byte i = 0; i < 20; i++){
      Serial.write(myFile.read());
    }
    
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
