/**
 * colors_ext.h - External color helpers borrowed from other projects.
 */

/// Drawing helpers
// (FUNCTION STOLEN FROM http://blog.saikoled.com/post/43693602826/why-every-led-light-should-be-using-hsi-colorspace on 10/27/14)
// This function will make it easier to create lighter and darker versions of the same color.
void hsi2rgb(float H, float S, float I, uint32_t* rgb) {
  int r, g, b;
  //H = fmod(H, 360); // cycle H around to 0-360 degrees
  H = ((int)(H * 100) % 360000)/100.0;; // cycle H around to 0-360 degrees
  H = 3.14159 * H / (float)180; // Convert to radians.
  S = S > 0 ? (S < 1 ? S : 1) : 0; // clamp S and I to interval [0,1]
  I = I > 0 ? (I < 1 ? I : 1) : 0;

  // Math! Thanks in part to Kyle Miller.
  if (H < 2.09439) {
    r = 255 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
    g = 255 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
    b = 255 * I / 3 * (1 - S);
  } else if (H < 4.188787) {
    H = H - 2.09439;
    g = 255 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
    b = 255 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
    r = 255 * I / 3 * (1 - S);
  } else {
    H = H - 4.188787;
    b = 255 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
    r = 255 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
    g = 255 * I / 3 * (1 - S);
  }
  *rgb = strip.Color(r,g,b);
}
///
///

// ----------------------------------------
// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
// ADAFruit Code - https://github.com/adafruit/Adafruit_NeoPixel
// ----------------------------------------
uint32_t wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}



// ----------------------------------------
// allsolid
// Sets all globes to a solid color.
// ----------------------------------------
void allSolid(uint32_t color){
  int i;
  //setStripSolid(strip0, color);
  for (i=0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, color);
  }
  strip.show();
}

// ----------------------------------------
// rainbowAll
// Walks all globes through the same colors.
// ----------------------------------------
int wheelVal = 0;
void rainbowAll() {
  allSolid(wheel(wheelVal));
  wheelVal++;
  if (wheelVal > 255) {
    wheelVal = 0;
  }
  delay(25);
}


// ----------------------------------------
// rainbow
// Draws tiny rainbows up and down the strips.
// ----------------------------------------
int colorSpeed = 3;
void rainbow() {
  uint32_t color, i;
  for (i=0; i < strip.numPixels(); i++){
    color = wheel(((wheelVal + i) * colorSpeed)  % 255);
    strip.setPixelColor(i, color);
  }
  strip.show();
  
  wheelVal++;
  if (wheelVal > 255) {
    wheelVal = 0;
  }
  delay(25);
}

bool showOdd = false;
void rainbowTheater() {
  uint32_t color, i;
  for (i=0; i < strip.numPixels(); i++){
    if ((i & 1) == 0){
      if (showOdd){
        color = wheel(((wheelVal + i) * colorSpeed)  % 255);
      }else{
        color = 0;
      }
    } else {
      if (showOdd){
        color = 0;
      }else{
        color = wheel(((wheelVal + i) * colorSpeed)  % 255);
      }      
    }
    strip.setPixelColor(i, color);
  }
  strip.show();


  if (showOdd) {
    showOdd = false;
  } else {
    showOdd = true;
  }
  
  wheelVal++;
  if (wheelVal > 255) {
    wheelVal = 0;
  }
  delay(100);
}

// Strandtest code:
// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.
void adaLoop() {
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

