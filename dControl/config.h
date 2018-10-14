//#define SETTINGS_ADK
#define SETTINGS_TEENSY2

#ifdef SETTINGS_ADK
const boolean isAnalog = false;
const int button1Pin = 6;
const int button2Pin = 7;
const int ledPin =  13;

const int MIC_PIN = 6;

const int leftPin = 5;
const int frontPin = 9;
const int rightPin = 6;
const int backPin = 10;
#endif

// Oh shit, is this rainbow snek 2?
#ifdef SETTINGS_TEENSY2
const boolean isAnalog = true;
const int button1Pin = 0;
const int button2Pin = 0;
const int pinA = A0;
const int pinB = A1;
const int pinC = A2;

const int ledPin =  13;

const int MIC_PIN = A6;

// 4x OUTPUT RAR!!!!
const int leftPin = 4;
const int frontPin = 5;
const int rightPin = 9;
const int backPin = 10;
#endif

#include "fastpat.h"

void setupButtons(){
  if (isAnalog) {
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    pinMode(pinC, INPUT);
    Serial.begin(115200);
  } else {
    // initialize the pushbutton pin as an input:
    pinMode(button1Pin, INPUT);
    pinMode(button2Pin, INPUT);
  }
}

unsigned long debounce = 0;
void buttonLoop() {
  if (isAnalog) {
    static int lastBright = 0;
    static int lastMode = 0;
    static int lastMod = 0;
    
    int currBright = (analogRead(pinA) / 10.24);
    int currMode = (analogRead(pinB) / (1024 / numModes));
    int currMod = (analogRead(pinC) / (1024 / 32));
    
    if (lastBright != currBright) {
      FastLED.setBrightness(currBright);
    }
    if (currMode != lastMode) {
      mode = currMode;
    }
    if (lastMod != currMod) {
      brightMod32 = lastMod;
    }

    Serial.println(String("\n\nA(brighness)") + 
        analogRead(pinA) + String("\nB(mode) ") + 
        analogRead(pinB) + String("\n:C(mod)") + 
        analogRead(pinC)); 

    
    Serial.println(String("\n\nbrighness)") + 
        currBright + String("(mode) ") + 
        currMode + String("\n:mod)") + 
        currMod);
  } else {
    // read the state of the pushbutton value:
    button1 = analogRead(button1Pin);
    button2 = analogRead(button2Pin);
  
    oneEnable = button1 < 100;
    twoEnable = button2 < 100;  
  
    if ((debounce + debounceMillis) < millis()){
      // Ignore others on brightness
      if (oneEnable && twoEnable) {
        brightMod32 = (brightMod32 + 1) % 32;
        if (brightMod32 < 4) {
          brightMod32 = 4;
        }
        FastLED.setBrightness(brightMod32 * (100/32));
        uiBlink(CRGB(0,0,0), CRGB::Gold);
        delay(100); // Dimmer!
      } else {    
        // Turn on the LED after pressing 1
        // Turn off after pressing 2, it's UI, basically
        if (oneEnable) {
          digitalWrite(ledPin, HIGH);
          oneMod32 = (oneMod32 + 1) % 32;
          oneMod255 = (oneMod255 + 1) % 255;
          uiBlink(CRGB(255,0,0), CRGB(0,0,255));
          while (analogRead(button1Pin) < 100) {
            delay(5);
          }
          FastLED.clear();
        }
        if (twoEnable) {
          digitalWrite(ledPin, LOW);
          modeMod = (modeMod + 1) % numModes;
          mode = modeMod;
          uiBlink(CRGB(0,255,0), CRGB(0,0,255));
          while (analogRead(button2Pin) < 100) {
            delay(5);
          }
          FastLED.clear();
        }
      }
      debounce = millis();
    }
  }
}
