/**
 * command.h - centralization of commanding structure and serial
 *             controls
 */
String readString, command, value;

// parser for commands and values
void parseCmdVal(String command, String value) {
  Serial.println("Command: " + command);  //print to serial monitor to see parsed results
  Serial.println("Value: " + value);

  // Mode
  if (command == "mode") {
    int newMode = value.toInt();
    Serial.println("Changing mode to:" + value);
    // TODO: validate mode value
    mode = newMode;
  }
}

// Loop called in main to manage serial communication
void serialLoop() {
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    } 
  }

  if (readString.length() >0) {
    Serial.println(readString); //see what was received
    
    // expect a string like 07002100 containing the two servo positions      
    command = readString.substring(0, 4); //get the first four characters
    value = readString.substring(4, 8); //get the next four characters 
      
    parseCmdVal(command, value);
    readString="";
  }   
}
