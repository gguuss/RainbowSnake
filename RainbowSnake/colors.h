/**
   colors.h - LED pattern rendering code for Adafruit Neopixel driver.
*/
#ifndef COLORS_H
#define COLORS_H

// -----------------------------------------------------------
// Sparkles the LEDs
// -----------------------------------------------------------
static void sparkle() {
  int numSparkles = random(0, maxSparkle);
  int* shownSparkles = new int(numSparkles);

  for (int sparkles = 0; sparkles < numSparkles; sparkles++) {
    shownSparkles[sparkles] = random(0, strip.numPixels());
    strip.setPixelColor(shownSparkles[sparkles], 0xFFFFFF);
  }
  strip.show();

  // Consider using a random delay, kinda sucks currently.
  //delay(random(40,70));
  //delay(100);
  delay(SYSDELAY);

  for (int sparkles = 0; sparkles < numSparkles; sparkles++) {
    strip.setPixelColor(shownSparkles[sparkles], 0x000000);
  }

  strip.show();

  delete(shownSparkles);
}

static void modeTweak(int currentMode) {
  switch (currentMode) {
    case COLOR_MODES::ADA_LOOP:
    case COLOR_MODES::RAINBOW_LOVE:
    case COLOR_MODES::MODE_TWEAK:
    case COLOR_MODES::HSV:
    case COLOR_MODES::VLOOP:
      switch (chaseSize) {
        case 7:
          chaseSize = 5;
          break;
        case 5:
          chaseSize = 3;
          break;
        case 3:
          chaseSize = 7;
          break;
        default:
          chaseSize = 5;
          break;
      }
      break;
    case COLOR_MODES::RAINBOW:
    case COLOR_MODES::RAINBOW_ALL:
    default:
      for (int i = 0; i < 25; i++) {
        sparkle();
        delay(random(5, 15));
      }
      break;
  }
}


static void basicChase() {
  strip.setPixelColor(head, color); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(SYSDELAY);                        // Pause 20 milliseconds (~50 FPS)

  if (++head >= strip.numPixels()) {        // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
    if ((color >>= 8) == 0)         //  Next color (R->G->B) ... past blue now?
      color = 0xFF0000;             //   Yes, reset to red
  }
  if (++tail >= strip.numPixels()) tail = 0; // Increment, reset tail index
}

static void drawPeers() {
  strip.clear();

  for (int i = 0; i < peerCount; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.show();
  delay(0);
}

// -----------------------------------------------------------
// Clears all pixels
// -----------------------------------------------------------
static void clearPixels() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
}


// -----------------------------------------------------------
// Tactical mode - sets red back, white front
// -----------------------------------------------------------
static void tacMode() {
  for (int i = 0; i < 5; i++) {
    strip.setPixelColor(i, 0xFFFFFF);
    strip.setPixelColor(strip.numPixels() - (i + 1), 0xFFFFFF);
  }

  for (int i = 36; i < 46; i++) {
    strip.setPixelColor(i, 0xFF0000);
  }
  strip.show();
}


// -----------------------------------------------------------
// Draws rainbows
// -----------------------------------------------------------
static void rainbowLove() {
  if (chaseUp) {
    for (int i = 0; i < strip.numPixels(); i++) {
      if (i >= chase && i < chase + chaseSize) {
        strip.setPixelColor(i, wheel(((255 / strip.numPixels()) * i) % 255));
      } else {
        strip.setPixelColor(i, 0);
      }
    }
    chase++;
    if (chase > strip.numPixels() - chaseSize) {
      //chase = 0;
      chaseUp = false;
    }
  } else {
    for (int i = strip.numPixels(); i >= 0; i--) {
      if (i >= chase && i < chase + chaseSize) {
        strip.setPixelColor(i, wheel(((255 / strip.numPixels()) * i) % 255));
      } else {
        strip.setPixelColor(i, 0);
      }
    }
    chase--;
    if (chase + chaseSize  < chaseSize) {
      //chase = 0;
      chaseUp = true;
    }
  }
  strip.show();
  delay(SYSDELAY);
}

// -----------------------------------------------------------
// Draws green -> red gradient for value % of the strip
// -----------------------------------------------------------
int vuDrawCounter = 0;
static void drawVu() {
  vuDrawCounter++;
  if (vuDrawCounter % 10 == 0) {
    vuPercent -= 1; // push down the VUs
  }
  int limit = (strip.numPixels() * vuPercent) / 100;

  // For green -> red gradient
  int step = 255 / strip.numPixels();

  // UGH, power kills this one
  strip.clear();
  for (int i = 0; i < strip.numPixels() && i < limit; i++) {
    strip.setPixelColor(i, 255 - (i * step), i * step, 0);
  }
  strip.show();
  delay(SYSDELAY);
}


// -----------------------------------------------------------
// Draws current brightness
// -----------------------------------------------------------
void showBrightness() {
  clearPixels();
  for (int i = 0; i < defaultBrightness / 25; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0));
    strip.show();
    delay(SYSDELAY * 5);
  }
}


// -----------------------------------------------------------
// Increases current brightness
// -----------------------------------------------------------
void increaseBrightness() {
  defaultBrightness += 10;
  if (defaultBrightness > 255) {
    defaultBrightness = 0;
  }
  strip.setBrightness(defaultBrightness); // 0...255
}


// -----------------------------------------------------------
// Decreases current brightness
// -----------------------------------------------------------
void decreaseBrightness() {
  defaultBrightness -= 10;
  strip.setBrightness(defaultBrightness); // 0...255s
}


static float hueStd = 1.0;
static float s = 1.0;
static float i = 1.0;
static void hsvLoop() {

  hueStd += 5;
  s += 5;
  hsi2rgb(hueStd, s, i,  &color);
  strip.setPixelColor(head, color); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(SYSDELAY);                        // Pause 20 milliseconds (~50 FPS)

  if (++head >= strip.numPixels()) {        // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
    hsi2rgb(hueStd, s, i,  &color);
    strip.setPixelColor(head, color); // 'On' pixel at head
  }
  if (++tail >= strip.numPixels()) tail = 0; // Increment, reset tail index
}


// Loops the V-value
int vLoopMode = 0;
static void vLoop() {
  switch (vLoopMode) {
    case 0:
      hueStd += 10;
      vLoopMode++;
      break;
    case 1:
      s += 5;
      if (s > 255) {
        s = 50;
        vLoopMode++;
      }
      break;
    case 2:
      i += 5;
      if (i > 255) {
        i = 50;
        vLoopMode = 0;
      }
      break;
  }
  hsi2rgb(hueStd, s, i,  &color);
  strip.setPixelColor(head, color); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(SYSDELAY);                  // Pause 20 milliseconds (~50 FPS)

  if (++head >= strip.numPixels()) {        // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
    hsi2rgb(hueStd, s, i,  &color);
    strip.setPixelColor(head, color); // 'On' pixel at head
  }
  if (++tail >= strip.numPixels()) tail = 0; // Increment, reset tail index
}

bool veeYouDirUp = true;
int veeYouWheel = 25;
int gMod = 0;
static void veeYou(int tick) {
  int mod = gMod;
  if (veeYouDirUp && mod != 0) {
    mod = (strip.numPixels() / 2) - mod;
  }
  for (int i = 0; i < (strip.numPixels() / 2) + 1; i++) {
    if (i < mod) {
      strip.setPixelColor(i, wheel(veeYouWheel));
      strip.setPixelColor(strip.numPixels() - i, wheel(255 - veeYouWheel));
    } else {
      strip.setPixelColor(i, 0);
      strip.setPixelColor(strip.numPixels() - i, 0);
    }
  }
  gMod++;
  if (gMod > strip.numPixels() / 2) {
    veeYouDirUp = veeYouDirUp ? false : true;
    veeYouWheel += 25;
    veeYouWheel = veeYouWheel % 255;
    gMod = 0;
  }
  strip.show();
  if (gBpm == 120) {
    delayMicroseconds(random(5000, 50000));
  }
  // Use BPM counter
  delay ((3600 / gBpm) / (strip.numPixels() / 2));
}

// From Magic
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void lineHelperWithHeight(uint32_t c, uint8_t offset, uint8_t wrapSize, uint16_t height) {
  uint16_t i;
  
  // Draw the line
  for(i=strip.numPixels(); i > strip.numPixels() - height; i -= wrapSize) {
    if (dir_trickle_up){
      strip.setPixelColor(i + offset, c);
    } else {
      strip.setPixelColor(strip.numPixels() - (i + offset), c);
    }
  }
}

void lineHelper(uint32_t c, uint8_t offset, uint8_t wrapSize) {
  uint16_t i;
  
  // Draw the line
  lineHelperWithHeight(c, offset, wrapSize, strip.numPixels());  
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Fill the dots one after the other with a color
void fastColorWipe(uint32_t c, uint8_t showSpeed) {
  #ifndef NOMOTION
  //motionLoop();
  #endif
  if (dir_trickle_up) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        if (i % showSpeed == 0) {
          strip.show();
        }
    }  
  }else{
    for(uint16_t i=strip.numPixels(); i > 0; i--) {
        strip.setPixelColor(i, c);
        if (i % showSpeed == 0) {
          strip.show();
        }
    }      
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      //if (isTrigger()) {
      //  return;
      //}
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256 && mode == 9; j++) {     // cycle all 256 colors in the wheel
    //handleWifiRequest();
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Draws a straight line
void drawStraightLineHelper(uint32_t c, uint8_t offset) {
  lineHelper(c, offset, 27);
}


void swirlLines(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4, uint8_t wait){
  // Clear the strip  
  strip.clear();
  
  lineHelper(c1, 0, 4);
  lineHelper(c2, 1, 4);
  lineHelper(c3, 2, 4);
  lineHelper(c4, 3, 4);  
  delay(wait);  
   
  // Show the lines
  strip.show();
}

void twistHelper(uint8_t minFudge, uint8_t maxFudge, uint8_t waitMin, uint8_t waitMax, int stepValue) {
  uint8_t colorFudge;
  for (colorFudge=minFudge; stepValue > 0 ? (colorFudge < maxFudge): (colorFudge > maxFudge); colorFudge += stepValue) {
    if (colorFudge & 1) {
      swirlLines(
        Wheel(colorFudge),       // Color 1
        0,                       // Color 2
        Wheel(colorFudge + 16),  // Color 3
        0,                       // Color 4
        random(waitMin,waitMax));                     // Delay
    } else{
      swirlLines(
        0,                       // Color 1
        Wheel(colorFudge),       // Color 2
        0,                       // Color 3
        Wheel(colorFudge + 16),  // Color 4
        random(waitMin,waitMax));                     // Delay
    }
  }
}

void fire(uint16_t heightMin, uint16_t heightFinish, uint8_t waitMin, uint8_t waitMax) {
  uint16_t i, j, height, temp;
  #ifndef NOMOTION
  //motionLoop();
  #endif
    
  strip.clear();
  height = random(heightMin, heightFinish);
  
  // Note: This function is using global did_trickle_up that I'm assuming is set in the main loop.
  //
  if (dir_trickle_up){
    for(i=0; i< strip.numPixels() && i < (height); i++) {      
      temp = i / 8;
      strip.setPixelColor(strip.numPixels() - i, strip.Color(127, temp, 0));
    }
    delay(0);
  }else{
    for(i=strip.numPixels() - 1; i > strip.numPixels() - height; i--) {      
      temp = i / 8;
      strip.setPixelColor(strip.numPixels() - i, strip.Color(127, temp, 0));
    }
    delay(0);
  }
  strip.show();
  delay(random(waitMin, waitMax));
}


void vineHelper(uint32_t color, uint8_t offsetFudge) {
  uint16_t growDelay = 50, vineSize1 = 0, vineSize2 = 0, vineSize3 = 0, vineSize4 = 0;
  uint8_t vineGrowMin = 1, vineGrowMax = 4;

  while (vineSize1 < strip.numPixels() && vineSize2 < strip.numPixels() && vineSize3 < strip.numPixels() && vineSize4 < strip.numPixels() ) {
    lineHelperWithHeight(color, 0 + offsetFudge, 4, vineSize1);  // Color, offset, wrapsize, height
    lineHelperWithHeight(color, 1 + offsetFudge, 4, vineSize2);  // Color, offset, wrapsize, height
    lineHelperWithHeight(color, 2 + offsetFudge, 4, vineSize3);  // Color, offset, wrapsize, height
    lineHelperWithHeight(color, 3 + offsetFudge, 4, vineSize4);  // Color, offset, wrapsize, height
    
    strip.show();
    
    vineSize1 += random(vineGrowMin, vineGrowMax);
    vineSize2 += random(vineGrowMin, vineGrowMax*2);
    vineSize3 += random(vineGrowMin, vineGrowMax);
    vineSize4 += random(vineGrowMin, vineGrowMax*3);
    //if (isTrigger()) {
    //  return;
    //}
    delay(growDelay);    
  }
  delay(growDelay * 10);
}


void drawStraightLinesTestColor(){
  uint8_t colorFudge, lineCounter;

  strip.clear();
  for (colorFudge=0; colorFudge < 255; colorFudge++) {
    for (lineCounter = 0; lineCounter < 27; lineCounter+=3){
      drawStraightLineHelper(Wheel(colorFudge + (lineCounter*5)), lineCounter);
    }
  }
  strip.show();
}


void drawStraightLinesTest(){
  uint8_t posFudge, lineCounter;

  for (posFudge=0; posFudge < 27; posFudge++) {
    strip.clear();
    drawStraightLineHelper(strip.Color(127,172,127), posFudge);
    drawStraightLineHelper(strip.Color(0,0,127), posFudge+4);
    strip.show();
    delay(60);
  }
}


void chips(uint8_t wait){
  uint8_t posFudge, lineCounter;

  strip.clear();
  drawStraightLineHelper(strip.Color(127,0,0), 0);
  strip.show();
  delay(wait);
  
  strip.clear();
  drawStraightLineHelper(strip.Color(0,0,127), 13);
  strip.show();
  delay(wait);
}

void flash(uint32_t c, uint8_t wait) { 
  strip.clear();
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
  strip.clear();
  strip.show();
}

void fill(uint32_t c, uint8_t wait) { 
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}


void lightning(){
  // TODO: For ... random # flashes ... 
  flash(strip.Color(127,127,127), random(5, 10));
  delay(random(25, 75));
  flash(strip.Color(127,127,127), random(5, 10));
  delay(random(25, 75));
  flash(strip.Color(127,127,127), random(5, 10));
  delay(random(1000, 5000));

  flash(strip.Color(127,127,127), random(5, 10));
  delay(random(25, 75));
  flash(strip.Color(127,127,127), random(5, 10));
  delay(random(1000, 5000));
  
  flash(strip.Color(127,127,127), random(5, 10));
  delay(random(250, 5000));
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    //if (!isTrigger()){
    //  break;
    //}
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void ring(uint32_t c, uint8_t ringsize, bool addReverse, uint8_t wait) {
  uint16_t i, j;
  
  #ifndef NOMOTION
  //motionLoop();
  #endif
  for(i=0; i< strip.numPixels(); i += ringsize) {
    strip.clear();
    if (dir_trickle_up){    
      for(j=0; j < ringsize; j++){
        strip.setPixelColor(i+j, c);      
      }
    } else {
      for(j=0; j < ringsize; j++){
        strip.setPixelColor((strip.numPixels() - (i+j)), c);      
      }
    }
    strip.show();
    delay(wait);
  }
  
  if (addReverse) {
    strip.clear();
    if (dir_trickle_up){
      for(i; i > 0; i -= ringsize) {
        for(j=0; j < ringsize; j++){
          strip.setPixelColor(i+j, c);      
        }
      }
    } else {
      for(j=0; j < ringsize; j++){
        strip.setPixelColor((strip.numPixels() - (i+j)), c);      
      }
    }
    strip.show();
    delay(wait);
  }
}

#endif
