/**
 * pov.h
 * Persistance of vision (POV) patterns.
 */

int row = 0;
enum POV_MODES {
  POV_RAINBOW_SPIRAL = 0,
  POV_DIAMONDS = 1,
  POV_SAW = 2
};

// Renders a rainbow spiral
void rainbowSpiral(){
  for (int i=0; i < strip.numPixels(); i++){
    if ((i + row) % 5 != 0){
      strip.setPixelColor(i, 0);
    }else{
      strip.setPixelColor(i, wheel(row*10));
    }
  }
  strip.show();
  row++;
}

// draws sawtooth pattern
void povDiamonds(boolean drawSaw) {
  for (int i=0; i < strip.numPixels(); i++){
    if ((i + row) % 5 != 0){
      strip.setPixelColor(i, 0);
    }else{
      strip.setPixelColor(i, wheel(row*10));
    }
    if ((i + row) % 7 != 0){
      strip.setPixelColor(i, 0);
    }
  }
  strip.show();
  row++;
}

unsigned long pictureLoopTime = 0;
int povMode = 1;
int numPovModes = 3;
void pictureLoop() {
  if (pictureLoopTime == 0){
    pictureLoopTime = millis();
  }

  switch(povMode) {
    case POV_RAINBOW_SPIRAL:
      rainbowSpiral();
      break;
    case POV_DIAMONDS:
      povDiamonds(false);
      break;
    case POV_SAW:
      povDiamonds(true);
      break;
  }

  if (millis() - pictureLoopTime  > 5000){
    povMode++;
    pictureLoopTime = 0;
    if (povMode >= numPovModes){
      povMode = 0;
    }
  }
  
  // draw hella fast
  delayMicroseconds(5000);
}

