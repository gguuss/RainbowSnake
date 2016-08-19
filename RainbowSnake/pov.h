#ifndef POV_H
#define POV_H
/**
 * pov.h
 * Persistance of vision (POV) patterns.
 */
 
#include "bitmap.h"

int row = 0;
enum POV_MODES {
  POV_RAINBOW_SPIRAL = 0,
  POV_DIAMONDS = 1,
  POV_SAW = 2, 
  POV_BMP = 3,
  POV_BMP25 = 4,
  POV_BMP16 = 5, 
  POV_BMP16_FIRE = 6
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

void povWave() {
  // BPM, Min, Max
  uint8_t waveSize = beatsin8(240, 5, (2*strip.numPixels()) / 3);
  strip.clear();
  for (int i=0; i < waveSize; i++){
    strip.setPixelColor(i, wheel(waveSize * 20));
  }
  strip.show();
}

// Note the row counter is used to enable disable the square
void povSquare() {

  uint8_t waveSize = beatsin8(120, 5, (4*strip.numPixels()) / 5);
  if (waveSize > strip.numPixels() / 2){
    waveSize = (4*strip.numPixels()) / 5;
  } else {
    waveSize = strip.numPixels() / 2;
  }
  
  strip.clear();
  for (int i=strip.numPixels() / 5; i < waveSize; i++){
    strip.setPixelColor(i, wheel( waveSize * 8));
  }
  strip.show();
  row++;
}

// Loop for 16-pixel wide bitmaps
void bitmapLoop16(uint32_t bitmap[][16], int height) { 
  if (row >= height) row = 0;
  strip.clear();
  for (int i=0; i < 16; i++){
    strip.setPixelColor(i, bitmap[row][i]);
  }
  strip.show();
  EVERY_N_MILLIS(5) { row++; }
}

// Loop for 25-pixel wide bitmaps
void bitmapLoop25(uint32_t bitmap[][25], int height) { 
  if (row >= height) row = 0;
  strip.clear();
  for (int i=0; i < 25; i++){
    strip.setPixelColor(i, bitmap[row][i]);
  }
  strip.show();
  EVERY_N_MILLIS(5) { row++; }
}

unsigned long pictureLoopTime = 0;
int povMode = 1;
int numPovModes = 7;
void pictureLoop() {
  if (pictureLoopTime == 0){
    pictureLoopTime = millis();
  }

  switch(povMode) {
    case POV_RAINBOW_SPIRAL:
      // FIXME
      //rainbowSpiral();
      //povSquare();
      bitmapLoop25(bitmap25_charlie, 25);
      break;
    case POV_DIAMONDS:
      //rainbowSpiral();
      bitmapLoop25(bitmap25_rainbow, 25);
      break;
    case POV_SAW:
      // FIXME
      //povDiamonds(true);
      //povWave();
      bitmapLoop25(bitmap25_mega, 25);
      break;
    case POV_BMP:
      //bitmapLoop16(bitmap16_1, 12);
      bitmapLoop25(bitmap25_snake, 25);
      break;
    case POV_BMP25:
      bitmapLoop25(bitmap25_fire, 25);
      break;
    case POV_BMP16:
      bitmapLoop16(bitmap16_pride, 54);
      break;
    case POV_BMP16_FIRE:
      bitmapLoop16(bitmap16_fire, 48);
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
  delayMicroseconds(0);
}
#endif
