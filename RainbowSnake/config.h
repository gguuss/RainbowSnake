/**
 *  LED / color / board Globals, I'm sorry.
 */

// Updates the mode to what's "next"
// TODO: settings go here
int mode = 1; // TODO: Rename, current color mode
int count = 0; // Display loop count global
int peerCount = 0; // Mesh peer count global

static bool hasModeswitch = true; // true

#define NUMM_MODES 9 // number of dotstar patterns
int numModes = NUMM_MODES; // SEE below enum
int numFastModes = 24; // set to last fast value
int numButtonClickerModes = 26; // set to number of clicker modes
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

  // Button Modes
  BUTTON_CLICKER = 25,
  GROW_CLICKER = 26,
  SOLIDCOLOR = 27
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
#define SETTINGS_WEMOS
//#define SETTINGS_ESP8266
//#define SETTINGS_ADK
//#define SETTINGS_NANO
//#define SETTINGS_HUZZAH8266


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

// Settings correspond to Feather/shield
#ifdef SETTINGS_HUZZAH8266
#define NOT_FAST
#define MODE_PIN   14  // mode select pin connected to ground
#define NUMPIXELS  25 // Number of LEDs in strip
#define DATAPIN    0
#define CLOCKPIN   16
#define SYSDELAY   50
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
#endif
//______________End HUZZAH8266 settings


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


