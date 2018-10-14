#include "FastLED.h"

// TODO(move to globals.h)
// Buttons globals
const int debounceMillis = 500; // Slow?
const int numModes = 12;

boolean oneEnable = false; // Not pressed == high
boolean twoEnable = false;

int button1 = 0;         // variable for reading the pushbutton status
int button2 = 0;
int buttonA = 0;
int buttonB = 0;
int buttonC = 0;
int oneMod32 = 0;
int oneMod255 = 0;
int mode = 0;
int modeMod = 0;
int brightMod32 = 24;

// from Rainbowsnek
byte* heat;
bool gReverseDirection = false;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
int gBpm = 120;

// LEDs
const int NUM_LEDS = 12; // 576?
#define FRAMES_PER_SECOND 60
CRGB leds[NUM_LEDS*4]; // 4 Strips!
// END TODO(move)

// Pin configuration
#include "config.h"
#include "vu.h"

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      

  Serial.begin(115200);
  Serial.println("Welcome to dControl.");

  // FastLED time.  
  FastLED.addLeds<WS2812B, leftPin, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, frontPin, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, rightPin, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, backPin, GRB>(leds, NUM_LEDS);

  // dimmer
  FastLED.setBrightness(100);

  // setup buttons
  setupButtons();

  // Mic
  setupVU();

  // Sup nerds
  bootAnimFast();
  FastLED.clear();
  FastLED.show();
}

void loop(){
  buttonLoop();
  ledLoop();
}

void ledLoop() {
  Serial.println(mode);
  switch (mode){
    case 0:
      pride();
      break;
    case 1:
      juggle();
      break;
    case 2:
      narwhal();
      break;      
    case 3:
      stackingMore();
      break;
    case 4:
      vuLoop();     
    case 5:
      redBeat();
      break;
    case 6:
      rgb();
      break;
    case 7:
      bounce();
      break;
    case 8:
      //fireLoop();
      mode++;
      break;
    case 9:
      sinelon();
      break;           
    case 10:
      bpm();
      break;   
    case 11:
      beatDetectLoop();
      break;
    case 12:
      colorwaves();
      break;      
    default:
      rainbow();

      // So lazy.
      if (mode > numModes) {
        mode = 0;
      }
      break;
  }
}
