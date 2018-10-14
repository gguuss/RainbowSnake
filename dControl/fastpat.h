#ifndef __FASTPAT_H__
#define __FASTPAT_H__

#include "GradientPalletes.h"

// bounce
#define GRAVITY           -9.81              // Downward (negative) acceleration of gravity in m/s^2
#define h0                1                  // Starting height, in meters, of the ball (strip length)
#define NUM_BALLS         4                  // Number of bouncing balls you want (recommend < 7, but 20 is fun in its own way)
float h[NUM_BALLS] ;                         // An array of heights
float vImpact0 = sqrt( -2 * GRAVITY * h0 );  // Impact velocity of the ball when it hits the ground if "dropped" from the top of the strip
float vImpact[NUM_BALLS] ;                   // As time goes on the impact velocity will change, so make an array to store those values
float tCycle[NUM_BALLS] ;                    // The time since the last time the ball struck the ground
int   pos[NUM_BALLS] ;                       // The integer position of the dot on the strip (LED index)
long  tLast[NUM_BALLS] ;                     // The clock time of the last ground strike
float COR[NUM_BALLS] ;                       // Coefficient of Restitution (bounce damping)




// Forward declarations of an array of cpt-city gradient palettes, and
// a count of how many there are.  The actual color palette definitions
// are at the bottom of this file.
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;

// Current palette number from the 'playlist' of color palettes
uint8_t gCurrentPaletteNumber = 0;

CRGBPalette16 gCurrentPalette( CRGB::Black);
CRGBPalette16 gTargetPalette( gGradientPalettes[0] );
// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


void Fire2012()
{
// Array of temperature readings at each simulation cell

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

void fireLoop()
{
  // Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());

  Fire2012(); // run simulation frame
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}


void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);

    // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  delay(0);
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV(gHue, 255, 192);

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  delay(0);
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}

void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = gBpm == 120 ? 62 : gBpm;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }

    // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  delay(0);
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}

void bounce() {
  for (int i = 0 ; i < NUM_BALLS ; i++) {
    tCycle[i] =  millis() - tLast[i] ;     // Calculate the time since the last time the ball was on the ground

    // A little kinematics equation calculates positon as a function of time, acceleration (gravity) and intial velocity
    h[i] = 0.5 * GRAVITY * pow( tCycle[i]/1000 , 2.0 ) + vImpact[i] * tCycle[i]/1000;

    if ( h[i] < 0 ) {                      
      h[i] = 0;                            // If the ball crossed the threshold of the "ground," put it back on the ground
      vImpact[i] = COR[i] * vImpact[i] ;   // and recalculate its new upward velocity as it's old velocity * COR
      tLast[i] = millis();

      if ( vImpact[i] < 0.01 ) vImpact[i] = vImpact0;  // If the ball is barely moving, "pop" it back up at vImpact0
    }
    pos[i] = round( h[i] * (NUM_LEDS - 1) / h0);       // Map "h" to a "pos" integer index position on the LED strip
  }

  //Choose color of LEDs, then the "pos" LED on
  for (int i = 0 ; i < NUM_BALLS ; i++) leds[pos[i]] = CHSV( uint8_t (i * 40) , 255, 255);
  FastLED.show();
  delay(0);
  //Then off for the next loop around
  for (int i = 0 ; i < NUM_BALLS ; i++) {
    leds[pos[i]] = CRGB::Black;
  }
}

void redBeat() {
  // BPM
  int pos = beatsin16(gBpm,0,NUM_LEDS);
  if (pos == 0){
    for (int i=0; i < NUM_LEDS; i++){
      if (i < NUM_LEDS / 3){
        leds[i] = CHSV(20, 255, 255);
      }else if (i < (NUM_LEDS/3)*2){
        leds[i] = CRGB::Red;
      } else {
        leds[i] = CHSV(10, 255, 255);
      }
    }
  } 
  if (pos % 3 == 0) {
    for (int i=0; i < NUM_LEDS; i++) {
      leds[i].fadeToBlackBy(1);
    }
  }

  FastLED.show();  
}

// ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
void colorwaves() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
  gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
      
  // uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    uint8_t index = hue8;
    //index = triwave8( index);
    index = scale8( index, 240);

    CRGB newcolor = ColorFromPalette(gCurrentPalette, index, bri8);

    nblend(leds[i], newcolor, 128);
  }
  
  FastLED.show();
}

void rainbow(){
  fill_rainbow(leds, NUM_LEDS, gHue, beatsin8(13+oneMod32,0,NUM_LEDS) + 10);
  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

// DemoReel100
void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8 + oneMod32; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();  
}

void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    nblend(leds[i], newcolor, 64);
  }
  FastLED.show();
}

void narwhal() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 50);  
  
  int pos = beatsin16(13,0,NUM_LEDS);
  int pos1 = beatsin16(26,0,NUM_LEDS);
  int pos2 = beatsin16(6,0,NUM_LEDS);

  leds[pos] += CHSV(gHue, 100, 255);
  leds[pos1] += CHSV(gHue, 150, 255);
  leds[pos2] += CHSV(gHue, 33, 255);

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  delay(0);
  FastLED.delay(3000/FRAMES_PER_SECOND); 
}

void fill() {
  fill_solid(leds, NUM_LEDS, CHSV(oneMod32 * (255/32), 255, 255));
  FastLED.show();
}

bool lightProtectMore = false;
bool fadeProtectMore = false;
int posLastMore = 0;
void stackingMore() {
  int gBpm = 70 + (oneMod32 * 3);
  
  // BPM
  int pos = beatsin16(gBpm,0,NUM_LEDS);
  if (pos == 0 && !lightProtectMore){
    CHSV color = CHSV(random8(), 255, 255);
    int stripeSize = (random8() % NUM_LEDS) / 3;
    int stripePos = posLastMore;
    if (stripePos < 0) stripePos = 0;
    for (int i=0; i < stripeSize && (i + stripePos) < NUM_LEDS; i++) {
      leds[i+ stripePos] = color;
    }
    // Latch
    lightProtectMore = true;
    posLastMore += stripeSize;
    if (posLastMore > NUM_LEDS) {
      posLastMore = 0;
    }
  } 
  if (pos == 1) {
    lightProtectMore = false;
  }
  
  // OMG it's so fast
  if (pos % (NUM_LEDS / 5) == 0 && !fadeProtectMore) {
    for (int i=0; i < NUM_LEDS; i++) {
      leds[i].fadeToBlackBy(16);
    }
    fadeProtectMore = true;
  } 
  if (pos % (NUM_LEDS / 5) != 0) {
    fadeProtectMore = false;
  }

  delay(0);
  FastLED.show();
}

void rgb() {
  for (int i=0; i < NUM_LEDS/4; i++) {
    if (i < (NUM_LEDS/4) / 3) {
      leds[i] = CRGB(255, 0, 0);      
    } else if (i < (2 * (NUM_LEDS/4)) / 3) {
      leds[i] = CRGB(0, 255, 0);      
    } else {
      leds[i] = CRGB(0, 0, 255);
    }
  }
  delay(0);
  FastLED.show();
}

int coolMod = 0;
void fillCool(int col) {
  for (int i=0; i < NUM_LEDS; i++){
    int value = (coolMod++ % NUM_LEDS) * (255/NUM_LEDS);
    leds[i] = CHSV(col, 255, value);
  }
  FastLED.show();
}

void bootAnimFast(){
  for (int i=0; i < NUM_LEDS; i++) {
    if (i%4 < 2) {
      leds[i] = CRGB(255, 0, 0);
    } else {
      leds[i] = 0;
    }
  }
  // Every other on.
  FastLED.show();
  FastLED.clear();
  delay(250);

  for (int i=0; i < NUM_LEDS; i++) {
    if (i%8 < 4) {
      leds[i] = CRGB(0, 255, 0);
    } else {
      leds[i] = 0;
    }
  }
  // Every other on.
  FastLED.show();
  FastLED.clear();
  delay(250);

  for (int i=0; i < NUM_LEDS; i++) {
    if (i%16 < 8) {
      leds[i] = CRGB(0, 0, 255);
    } else {
      leds[i] = 0;
    }
  }
  FastLED.show();
  FastLED.clear();
  delay(250);
}

void bootAnimTest(){
  for (int i=0; i < NUM_LEDS; i++) {
    if (i%4 < 2) {
      leds[i] = CRGB(255, 0, 0);
    } else {
      leds[i] = 0;
    }
  }
  // Every other on.
  FastLED.show();
  delay(80);
  for (int i=0; i < NUM_LEDS - 2; i++) {    
    leds[i+2] = leds[i+1];
    leds[i] = 0;
    FastLED.show();
  }

  for (int i=0; i < NUM_LEDS; i++) {
    if (i%8 < 4) {
      leds[i] = CRGB(0, 255, 0);
    } else {
      leds[i] = 0;
    }
  }
  // Every other on.
  FastLED.show();
  delay(80);
  for (int i=0; i < NUM_LEDS - 2; i++) {    
    leds[i+2] = leds[i+1];
    leds[i] = 0;
    FastLED.show();
  }

  for (int i=0; i < NUM_LEDS; i++) {
    if (i%16 < 8) {
      leds[i] = CRGB(0, 0, 255);
    } else {
      leds[i] = 0;
    }
  }
  FastLED.show();
  delay(80);
  for (int i=0; i < NUM_LEDS - 2; i++) {    
    leds[i+2] = leds[i+1];
    leds[i] = 0;
    FastLED.show();
  }
}

void uiBlink(CRGB one, CRGB two){
  fill_solid(leds, NUM_LEDS, 0);
  fill_solid(leds, 32, one);
  FastLED.show();
  delay(50);
  fill_solid(leds, 32, two);
  FastLED.show();
  delay(80);
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

#endif
