/**
 * command.h - centralization of commanding structure and serial
 *             controls
 * 
 * Commands are hardcoded currently on serial communication to
 *     mode - value : where the value is the integer of the mode constant 
 *     bryt - value : where the value is the brightness 0-255
 *     size - value : where the value is the size of the strip
 */
String readString, command, value;

#include <FS.h>
#include <EEPROM.h>

void loadSize() {
  NUM_LEDS = EEPROM.read(5);
  if (NUM_LEDS <= 0) {
    NUM_LEDS = 25;
  }
  #ifndef FAST_NEOPIXEL
  strip.updateLength(NUM_LEDS);
  #endif
}

void saveSize() {
  EEPROM.write(5, NUM_LEDS); // Store 
  EEPROM.commit();
}

/**
 * updateSize - Renders current selection for strip size.
 * 
 * @param currentSize The current size, in pixels of the LED strip.
 */
void updateSize(int currentSize) {
  #ifndef FAST_NEOPIXEL // TODO: determine update size NeoPixel
  strip.updateLength(currentSize * 5);
  #endif

  delay(0);
  for (int i=0; i < strip.numPixels(); i++) {
    if (i == 0 || i == strip.numPixels() - 1) {
      strip.setPixelColor(i, 0xFFFFFF); // white ends
    } else {
      strip.setPixelColor(i, 0);
    }
  } 
  strip.show();

  NUM_LEDS = currentSize * 5;
  saveSize();
  fastLedSetup();
}

void setSize(int newSize) {
  NUM_LEDS = newSize;
  //#ifndef FAST_NEOPIXEL
  //strip.updateLength(NUM_LEDS);
  //#endif
  //fastLedSetup();
}

// Current brightness settings
const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
int brightnessIndex = 0;
uint8_t brightness = brightnessMap[brightnessIndex];

// adjust the brightness, and wrap around at the ends
void adjustBrightness(bool up)
{
  if (up)
    brightnessIndex++;
  else
    brightnessIndex--;

  // wrap around at the ends
  if (brightnessIndex < 0)
    brightnessIndex = brightnessCount - 1;
  else if (brightnessIndex >= brightnessCount)
    brightnessIndex = 0;

  EEPROM.write(0, brightness);
  EEPROM.commit();
}

void setBrightness(int value)
{
  // don't wrap around at the ends
  if (value > 255)
    value = 255;
  else if (value < 0) value = 0;

  brightness = value;

  FastLED.setBrightness(brightness);
  strip.setBrightness(brightness);

  EEPROM.write(0, brightness);
  EEPROM.commit();
}

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
  // Brightness
  } else if (command == "bryt") {
    int newBrite = value.toInt();
    Serial.println("Changing brightness to:" + value);
    // TODO: validate brightness value
    setBrightness(newBrite);
  } else if (command == "size") {
    int newSize = value.toInt();
    Serial.println("Changing size to:" + value);
    setSize(newSize);
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
