#include "colors.h"
#include "tests.h"
#ifndef MAGICSTICK_H
#define MAGICSTICK_H


//#define NOBUTTONS
//#define NOLIGHTS
#define NOMOTION // or...

// Pins for buttons
//#define INPUT_HOLD D2 // WARNING: I'm seeing values as either 8 or 8+7 (HWBug?)
//#define INPUT_TRIGGER D7
// TODO: Figure out how to gracefully disable/enable button support

// TODO: Add pattern hold-style loops here
void holdPatternLoop(){
}

// TODO: Add interrupt switching here with loop rotation
void interruptPatternLoop(){
}


// Returns true if hold is pressed
// Note that the high value is "unpressed"
bool magicModePressed() {
  if (digitalRead(MODE_PIN) != HIGH) {
    return true;
  }
  return false;
}

// Returns true if trigger pressed
// Note that the high value is "unpressed"
bool isTrigger() {

  // TODO: button?  from app?
  return false;
}

// Temp value used to nudge colors
uint8_t colorFudge = 0;

void elements () {
  lightning();  
  iceTest();
  fireTest();
  // broken
  //arcaneTest();
  earth();
}

int magicTick = 0;
void inputTester() {  // for debug
  while(true){
    if (analogRead(D7) == 0){ // TRIGGER
      delay(50);
      //rainbowCycle(magicTick++);
      Serial.println("HI");
    } else {
      Serial.println("LO");
    }
    
    if (magicModePressed()){
      Serial.println("YO");
    }
    delay(20);
  }
}

// The Main application loop. Called after Setup().
int magicMode = 0, magicModeLimit = 10;
void magicLoop() {
  magicTick++;
  delay(0);
  serverLoop();
  delay(0);

  if (magicModePressed()){
    magicMode++;
    delay(200);
    strip.clear();
    strip.setPixelColor(1,255, 0, 255);
    strip.show();
    Serial.println(magicMode);
    delay(500);
  }

  switch (magicMode) {
    case 0:
      earth();
      break;
    case 1:
      fireTest();
      break;
    case 2:
      iceTest();
      break;
    case 3:
      lightning();
      break;
    default:
      magicMode = 0;
      break;
  }
  
  // Test All  "magic" elements
  //elements();
  delay(0);
}

#endif
