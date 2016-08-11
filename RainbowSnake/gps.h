/**
 * gps.h - All GPS related functions
 */


int compassThreshold = 2;
bool compassInTarget(int target) {
  int difference = compassDir - target;
  
  if (difference < 0) { difference *= -1; }

  if (difference < compassThreshold) {
    return true;    
  }
  return false;
}

void compass() {
  for (int i=0; i < strip.numPixels(); i++) {
    if (compassInTarget(0 + i - (strip.numPixels() / 2))) { // North
      strip.setPixelColor(i, 255, 0, 0);
    } else if (compassInTarget(90 + i - (strip.numPixels() / 2))) { // West
      strip.setPixelColor(i, 0, 255, 0);
    } else if (compassInTarget(180 + i - (strip.numPixels() / 2))) { // South
      strip.setPixelColor(i, 255, 255, 255);
    } else if (compassInTarget(270 + i - (strip.numPixels() / 2))) { // East
      strip.setPixelColor(i, 0, 0, 255);
    } else {
      strip.setPixelColor(i, 0, 0, 0);  
    }
  }
  strip.show();
  delay(20);
}

