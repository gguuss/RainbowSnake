/**
 * led_mesh.ino - Enables mesh-networked LED controllers with server interface, chat network,
 *    and sensor forwarding from mobile devices.
 */

#include <ESP8266WiFi.h>
String manageRequest(String request);
#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>

#define FASTLED_FORCE_SOFTWARE_SPI
#define FASTLED_FORCE_SOFTWARE_PINS
#include <FastLED.h>
#include "config.h"

// Includes for dependent color functions
#include "fastled.h"
#include "colors_ext.h"
#include "colors.h"
#include "colors_button.h"
#include "pov.h"
#include "gps.h"

// Serial command module and (eventually) command centralization
#include "command.h"

// server
#include "server.h"

//Magic staff/stick
#include "mpu.h" 
#include "tests.h"
#include "magicstick.h"

// POI- emulation of flowtoys pod poi
#include "poi.h"

// plz fix, ordering matters.
// Everything needs an ifndef, basically
#include "mesh.h"
#include "settings.h"
#include "button.h"
#include "control.h"

/**
 * Globals, #FIXME
 */
#define DISPCOUNT 10
int debounce = 0;
int countLimit = NUMPIXELS * DISPCOUNT; // change mode after 10x
int tick = 0;
boolean firstPictureLoop = true;

void setup() {
  #ifdef BURNINTEST
    strip.begin(); // Initialize pins for output
    strip.show();  // Turn all LEDs off ASAP
    strip.setBrightness(255); // Max brightness
    while(true) {
      rainbow(); // vary colors as much as possible
    }
  #endif

  #ifdef NOMODESWITCH
  hasModeswitch = false;
  #endif
  // Start the serial monitor
  Serial.begin(115200);

  strip.begin(); // Initialize pins for output
  strip.setBrightness(20);
  strip.show();  // Turn all LEDs off ASAP
  // TODO: Boot animation
  for (int i=0; i < strip.numPixels() / 2; i++){
    veeYou(i);
  }
  delay(3000);

  // not sure about this one...
  // used for the button
  //pinMode(MODE_PIN, INPUT);
  pinMode(MODE_PIN, INPUT_PULLUP);

  // test out old staff stuff
  //#define MAGICTEST
  #ifdef MAGICTEST
  while (true){
    magicLoop();
  }
  #endif


  fastLedSetup();

  setupServer();

  // 0 - No mode from settings...loaded in setupServer
  // >numFastModes - you will not be able to change the mode with button
  if (true || mode == 0 || mode > numFastModes) { 
    mode = notFast ? VEE_YOU : NARWHAL;
  }
  //mode = numButtonClickerModes;

  // for testing notification
  //hasNotification = true;
}


/**
 * loop()
 *
 * Arduino loop, performs the following:
 * 1. Checks web server
 * 2. Renders LED pattern
 * 3. Checks mode button and updates mode accordingly
 */
// FIXME: bad
static unsigned long lastModeChange = millis();
int nextmode = 0;
void loop() {
  tick++;
  delay(0);
  serverLoop();
  delay(0);
  serialLoop();

  // FIXME: Remove middleware, use currentPatternIndex
  switch (mode) {
    case COLOR_MODES::ADA_LOOP:
      adaLoop();
      nextmode = COLOR_MODES::RAINBOW_LOVE;
      break;
    case COLOR_MODES::RAINBOW_LOVE:
      rainbowLove();
      nextmode = COLOR_MODES::RAINBOW;
      break;
    case COLOR_MODES::MODE_TWEAK:
      mode++;
    // no break;
    case COLOR_MODES::RAINBOW:
      rainbow();
      nextmode = COLOR_MODES::HSV;
      break;
    case COLOR_MODES::HSV:
      hsvLoop();
      nextmode = COLOR_MODES::RAINBOW_ALL;
      break;
    case COLOR_MODES::VLOOP:
      vLoop();
      break;
    case COLOR_MODES::RAINBOW_ALL:
      rainbowAll();
      nextmode = COLOR_MODES::RAINBOW_THEATER;
      break;
    case COLOR_MODES::RAINBOW_THEATER:
      rainbowTheater();
      nextmode = COLOR_MODES::VEE_YOU;
      break;
    case COLOR_MODES::VEE_YOU:
      veeYou(tick);
      nextmode = COLOR_MODES::FAST_RED_BLUE;
      break;
    case PICTURE_LOOP:
      pictureLoop();
      break;
    case COLOR_MODES::FAST_RED_BLUE:
      if (tick % 50 < 25) {
        fastRedBlue(false);
      } else {
        fastRedBlue(true);
      }
      nextmode = FAST_RAINBOW;
      break;
    case FAST_RAINBOW:
      fastRainbow();
      nextmode = RAINBOW_GLITTER;
      break;
    case RAINBOW_GLITTER:
      rainbowWithGlitter();
      nextmode = CONFETTI;
      break;
    case CONFETTI:
      confetti();
      nextmode = SINELON;
      break;
    case SINELON:
      sinelon();
      nextmode = BPM;
      break;
    case BPM:
      bpm();
      nextmode = JUGGLE;
      break;
    case JUGGLE:
      juggle();
      nextmode = FIRE;
      break;
    case FIRE:
      fireLoop();
      nextmode = BOUNCE;
      break;
    case BOUNCE:
      bounce();
      nextmode = STACK_LIGHTS;
      break;
    case STACK_LIGHTS:
      stacking();
      nextmode = STACK_LIGHTS_MORE;
      break;
    case STACK_LIGHTS_MORE:
      stackingMore();
      nextmode = REDBEAT;
      break;
    case REDBEAT:
      redBeat();
      nextmode = BUBBLE_UP;
      break;
    case BUBBLE_UP:
      bubble(CRGB::Black, false);
      nextmode = COLOR_WAVES;
      break;
    case COLOR_WAVES:
      EVERY_N_SECONDS( 10 ) {
        gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
        gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
      }

      EVERY_N_MILLISECONDS(40) {
        nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 16);
      }
      colorwaves();
      nextmode = NARWHAL;
      break;
    case NARWHAL:
      narwhal();
      nextmode = PRIDE;
      break;
    case PRIDE:
      pride();
      nextmode = WIZARD;
      break;
    case SOLIDCOLOR:
      showSolidColor();
      break;
    case BUTTON_CLICKER:
      buttonClicker();
      break;
    case GROW_CLICKER:
      growClicker();
      break;
    case COMPASS:
      compass();
      break;
    case FINDWAYPOINT:
      distanceHome();
      break;
    case BUTTON_MESHCOUNT:
      buttonCountMeshNodes();
      break;
    case VU_METER:
      drawVu();
      break;
    case WIZARD:
      magicLoop();
      nextmode = COLOR_MODES::ADA_LOOP;
      break;
    case POI:
      poiLoop();
      break;
    default:
      mode = 0;
      break;
  }

  // Change the mode
  if (hasModeswitch && (mode <= numFastModes || mode == SOLIDCOLOR)) {
    // Not even!
    checkModeButton();
  } else {
    if ((millis() - lastModeChange) > 20000) {
      mode = nextmode;
      lastModeChange=millis();
    }
  }

  checkNotify();

  // Move along, nothing to see here...
  count++;
}
