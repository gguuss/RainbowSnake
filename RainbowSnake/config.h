#ifndef CONFIG_H
#define CONFIG_H
/**
    LED / color / board Globals, I'm sorry.
*/

// From Magic
bool dir_trickle_up = false;
void updateTrickleUp(float angle_y) {
  if (angle_y > 20) {
    dir_trickle_up = true;
  } else if (angle_y < -20) {
    dir_trickle_up = false;
  }
}


// Updates the mode to what's "next"
// TODO: settings go here
int mode = 1; // TODO: Rename, current color mode
int count = 0; // Display loop count global
int peerCount = 0; // Mesh peer count global
int compassDir = 0; // heading
static bool hasModeswitch = true; // true
boolean hasNotification = false;
double latitude = 0.0, longitude = 0.0; // For GPS stored position
double currLat = 0.0, currLong = 0.0; // For GPS current position
int vuPercent = 0;

#define NUMM_MODES 9 // number of dotstar patterns
int numModes = NUMM_MODES; // SEE below enum
int numFastModes = 25; // set to last fast value
int numButtonClickerModes = 33; // set to number of clicker modes
enum COLOR_MODES {
  // Adafruit modes
  ADA_LOOP = 0,
  RAINBOW_LOVE = 1,
  MODE_TWEAK = 2,
  RAINBOW = 3,
  HSV = 4,
  VLOOP = 5,
  RAINBOW_ALL = 6,
  RAINBOW_THEATER = 7,
  VEE_YOU = 8,
  PICTURE_LOOP = 9,

  // FASTLED MODES
  FAST_RED_BLUE = 10,
  FIRE = 11,
  FAST_RAINBOW = 12,
  RAINBOW_GLITTER = 13,
  CONFETTI = 14,
  SINELON = 15,
  BPM = 16,
  JUGGLE = 17,
  BOUNCE = 18,
  STACK_LIGHTS = 19,
  REDBEAT = 20,
  STACK_LIGHTS_MORE = 21,
  BUBBLE_UP = 22,
  COLOR_WAVES = 23,
  PRIDE = 24,
  NARWHAL = 25,

  // Button Modes
  BUTTON_CLICKER = 26,
  GROW_CLICKER = 27,
  COMPASS = 28,
  FINDWAYPOINT = 29,
  BUTTON_MESHCOUNT = 30,
  VU_METER = 31,
  WIZARD = 32,
  POI = 33,

  // Caution!!! Must be last
  SOLIDCOLOR = 34
};

// TWEAK ME!!!
int defaultBrightness = 30; // 0 ... 255
const int maxSparkle = 2;
int chase = 0;
int chaseSize = 5;
bool chaseUp = true;
// Globals reused from the strandtest code...
// FIXME: clean these up
int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

// BOARD SETTINGS -- full configurations below
// lights not working?  make sure you have the right board uncommented
#define SETTINGS_ESP32_THING
//#define SETTINGS_WEMOS
//#define SETTINGS_WEMOS_NEOPIXEL // most common, if gguuss gave you one... this.
//#define SETTINGS_ESP8266
//#define SETTINGS_ADK
//#define SETTINGS_NANO
//#define SETTINGS_HUZZAH8266
//#define SETTINGS_HUZZAH8266_FIXED // Hats, shoes, poi


// Settings correspond to WeMos/1Button
#ifdef SETTINGS_WEMOS
#define MODE_PIN   D3  // mode select pin connected to ground
#define NUMPIXELS  25 // Number of LEDs in strip
#define DATAPIN    D6
#define CLOCKPIN   D7
#define SYSDELAY   50
Adafruit_DotStar strip = Adafruit_DotStar(
                           NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
#endif
//______________End WeMos settings

// Settings correspond to ESP32 Thing
#ifdef SETTINGS_ESP32_THING
#define MODE_PIN   25  // mode select pin connected to ground
#define NUMPIXELS  60 // Number of LEDs in strip
#define DATAPIN    23
#define CLOCKPIN   22
#define SYSDELAY   50
#define NOMODESWITCH
Adafruit_DotStar strip = Adafruit_DotStar(
                           NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
#define NOWIFI
#define USEPREFS
#endif


// Settings correspond to WeMos/1Button
#ifdef SETTINGS_WEMOS_NEOPIXEL
#define MODE_PIN   D3  // mode select pin connected to ground
#define NUMPIXELS  50 // Number of LEDs in strip
#define DATAPIN    D6
#define SYSDELAY   50
#define FAST_NEOPIXEL true
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, DATAPIN, NEO_RGB + NEO_KHZ800);
#endif
//______________End WeMos settings

// Settings correspond to HUZZAH 8266
#ifdef SETTINGS_HUZZAH8266_FIXED
#define MODE_PIN   2  // mode select pin connected to ground
#define NUMPIXELS  25 // Number of LEDs in strip
#define DATAPIN    14
#define CLOCKPIN   12
#define SYSDELAY   50
#define NOMODESWITCH
Adafruit_DotStar strip = Adafruit_DotStar(
                           NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
#endif
//______________End ESP8266 settings


// Settings correspond to ESP8266
#ifdef SETTINGS_ESP8266
#define MODE_PIN   2  // mode select pin connected to ground
#define NUMPIXELS  25 // Number of LEDs in strip
#define DATAPIN    5
#define CLOCKPIN   4
#define SYSDELAY   500
#define NOMODESWITCH
Adafruit_DotStar strip = Adafruit_DotStar(
                           NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_GBR);
#endif
//______________End ESP8266 settings

// These settings correspond to the Ardino nano / USBAsp
#ifdef SETTINGS_NANO
#define MODE_PIN   3 // mode select pin connected to ground
#define NUMPIXELS 30 // Number of LEDs in strip
#define DATAPIN    4
#define CLOCKPIN   5
#define SYSDELAY   20
Adafruit_DotStar strip = Adafruit_DotStar(
                           NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
#endif
//______________End NANO settings

// These settings correspond to the Ardino ADK
#ifdef SETTINGS_ADK
#define MODE_PIN   3 // mode select pin connected to ground
#define NUMPIXELS  12 // Number of LEDs in strip
#define DATAPIN    11
#define CLOCKPIN   12
#define SYSDELAY   50
Adafruit_DotStar strip = Adafruit_DotStar(
                           NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_RGB);

#endif
//______________End ADK settings




/**
   Notification loop, exponentially backs off
*/
unsigned long lastNotification = 0;
int backOffCounter = 2;

/**
   Resets notofication variables.
*/
void resetNotify() {
  lastNotification = 0;
  backOffCounter = 2;
}

/**
   Exponentially backing off pattern that notifies the user
   that they have been notified.
*/
void checkNotify() {
  uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF};
  if (hasNotification && ((millis() - lastNotification) > backOffCounter * 1000) ) {
    strip.clear();
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 3; j++) {
        strip.setPixelColor(j, colors[i % 3]);
      }
      strip.show();
      delay(100);
      strip.clear();
      strip.show();
      delay(50);
    }
    lastNotification = millis();
    backOffCounter *= 2;
  }
}
#endif
