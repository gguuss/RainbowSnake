/**
 * led_mesh.ino - Enables mesh-networked LED controllers with server interface, chat network,
 *    and sensor forwarding from mobile devices.
 */

#include <ESP8266WiFi.h>
String manageRequest(String request);
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

// server
#include "server.h"

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
  strip.show();  // Turn all LEDs off ASAP
  // TODO: Boot animation
  for (int i=0; i < strip.numPixels() / 2; i++){
    veeYou(i);
  }
  delay(500);

  // not sure about this one...
  // used for the button
  //pinMode(MODE_PIN, INPUT);
  pinMode(MODE_PIN, INPUT_PULLUP);

  fastLedSetup();

  setupServer();

  mode = notFast ? VEE_YOU : STACK_LIGHTS;
  //mode = numButtonClickerModes;

  // for testing notification
  hasNotification = true;
}


/**
 * loop()
 *
 * Arduino loop, performs the following:
 * 1. Checks web server
 * 2. Renders LED pattern
 * 3. Checks mode button and updates mode accordingly
 */
void loop() {
  tick++;
  delay(0);
  serverLoop();
  delay(0);

  switch (mode) {
    case COLOR_MODES::ADA_LOOP:
      adaLoop();
      break;
    case COLOR_MODES::RAINBOW_LOVE:
      rainbowLove();
      break;
    case COLOR_MODES::MODE_TWEAK:
      mode++;
    // no break;
    case COLOR_MODES::RAINBOW:
      rainbow();
      break;
    case COLOR_MODES::HSV:
      hsvLoop();
      break;
    case COLOR_MODES::VLOOP:
      vLoop();
      break;
    case COLOR_MODES::RAINBOW_ALL:
      rainbowAll();
      break;
    case COLOR_MODES::RAINBOW_THEATER:
      rainbowTheater();
      break;
    case COLOR_MODES::VEE_YOU:
      veeYou(tick);
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
      break;
    case FAST_RAINBOW:
      fastRainbow();
      break;
    case RAINBOW_GLITTER:
      rainbowWithGlitter();
      break;
    case CONFETTI:
      confetti();
      break;
    case SINELON:
      sinelon();
      break;
    case BPM:
      bpm();
      break;
    case JUGGLE:
      juggle();
      break;
    case FIRE:
      fireLoop();
      break;
    case BOUNCE:
      bounce();
      break;
    case STACK_LIGHTS:
      stacking();
      break;
    case STACK_LIGHTS_MORE:
      stackingMore();
      break;
    case REDBEAT:
      redBeat();
      break;
    case BUBBLE_UP:
      bubble(CRGB::Black, false);
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
      break;
    case PRIDE:
      pride();
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
    default:
      mode = 0;
      break;
  }

  // Change the mode
  if (hasModeswitch && (mode <= numFastModes || mode == SOLIDCOLOR)) {
    // Not even!
    checkModeButton();
  } else {
    /*
    if (count > countLimit) {
      updateMeshMode();
    }
    */
  }

  checkNotify();

  // Move along, nothing to see here...
  count++;
}
