/**
 * colors.h - LED pattern rendering code for Adafruit Neopixel driver.
 */


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
  switch(currentMode){
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
      for (int i=0; i < 25; i++) {
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

static void drawPeers(){
  strip.clear();
  
  for (int i=0; i < peerCount; i++){
    strip.setPixelColor(i, strip.Color(255,0,0));
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
// Draws current brightness
// -----------------------------------------------------------
void showBrightness(){  
  clearPixels();
  for (int i=0; i < defaultBrightness / 25; i++){
    strip.setPixelColor(i, strip.Color(0,255,0));    
    strip.show();
    delay(SYSDELAY*5);
  }
}


// -----------------------------------------------------------
// Increases current brightness
// -----------------------------------------------------------
void increaseBrightness(){
  defaultBrightness += 10;
  if (defaultBrightness > 255){
    defaultBrightness = 0;
  }
  strip.setBrightness(defaultBrightness); // 0...255
}


// -----------------------------------------------------------
// Decreases current brightness
// -----------------------------------------------------------
void decreaseBrightness(){
  defaultBrightness -= 10;
  strip.setBrightness(defaultBrightness); // 0...255s
}


static float hueStd=1.0;
static float s=1.0;
static float i=1.0;
static void hsvLoop() {

  hueStd+=5;
  s+=5;
  hsi2rgb(hueStd, s, i,  &color);
  strip.setPixelColor(head, color); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(SYSDELAY);                        // Pause 20 milliseconds (~50 FPS)

  if(++head >= strip.numPixels()) {         // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
    hsi2rgb(hueStd, s, i,  &color);
    strip.setPixelColor(head, color); // 'On' pixel at head
  }
  if(++tail >= strip.numPixels()) tail = 0; // Increment, reset tail index
}


// Loops the V-value
int vLoopMode = 0;
static void vLoop() {
  switch(vLoopMode){  
    case 0:
     hueStd+=10;
      vLoopMode++;
      break;
    case 1:
      s+=5;
      if (s > 255){
        s = 50;
        vLoopMode++;
      }
      break;
    case 2:
      i+=5;
      if (i > 255){
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

  if(++head >= strip.numPixels()) {         // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
    hsi2rgb(hueStd, s, i,  &color);
    strip.setPixelColor(head, color); // 'On' pixel at head
  }
  if(++tail >= strip.numPixels()) tail = 0; // Increment, reset tail index
}

bool veeYouDirUp = true;
int veeYouWheel = 25;
int gMod = 0;
static void veeYou(int tick) {
  int mod = gMod;
  if (veeYouDirUp && mod != 0){
      mod = (strip.numPixels() / 2) - mod;
  }
  for (int i=0; i < (strip.numPixels() / 2) + 1; i++){
    if (i < mod) {
      strip.setPixelColor(i, wheel(veeYouWheel));
      strip.setPixelColor(strip.numPixels() - i, wheel(255-veeYouWheel));
    } else {
      strip.setPixelColor(i, 0);
      strip.setPixelColor(strip.numPixels() - i, 0);
    }
  }
  gMod++;
  if (gMod > strip.numPixels() / 2){
    veeYouDirUp = veeYouDirUp ? false : true;
    veeYouWheel += 25;
    veeYouWheel = veeYouWheel % 255;
    gMod = 0;
  }
  strip.show();
  if (gBpm == 120){
    delayMicroseconds(random(5000, 50000));
  }
  // Use BPM counter
  delay ((3600/gBpm) / (strip.numPixels() / 2));
}

