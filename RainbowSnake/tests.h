#ifndef TESTS_H
#define TESTS_H

bool isTrigger();
bool magicModePressed();
void motionLoop();

void iceTest() {
  // Draw slow wipe
  fastColorWipe(strip.Color(0, 0, 255), 5); // Blue
  fastColorWipe(strip.Color(0, 0, 0), 5); // Blue
  if (isTrigger() || magicModePressed()) {
    return;
  }

  // draw rings
  ring(strip.Color(0, 0, 255), 4, false, 0);
  ring(strip.Color(0, 0, 255), 4, false, 0);
  ring(strip.Color(0, 0, 255), 4, false, 0);
  if (isTrigger() || magicModePressed()) {
    return;
  }
    
  ring(strip.Color(0, 0, 255), 8, false, 0);
  ring(strip.Color(0, 0, 255), 8, false, 0);
  ring(strip.Color(0, 0, 255), 8, false, 0);  
  if (isTrigger() || magicModePressed()) {
    return;
  }
    
  ring(strip.Color(0, 0, 255), 16, false, 0);
  ring(strip.Color(0, 0, 255), 16, false, 0);
  ring(strip.Color(0, 0, 255), 16, false, 0);  
  if (isTrigger() || magicModePressed()) {
    return;
  }
    
  ring(strip.Color(0, 0, 255), 24, true, 0);
  ring(strip.Color(0, 0, 255), 24, true, 0);
  ring(strip.Color(0, 0, 255), 24, true, 0);
  ring(strip.Color(0, 0, 255), 24, true, 0);  
  if (isTrigger() || magicModePressed()) {
    return;
  }
    
  flash(strip.Color(0, 0, 255), 100);
  if (isTrigger() || magicModePressed()) {
    return;
  }
  delay(random(100, 200));
  if (isTrigger() || magicModePressed()) {
    return;
  }
  flash(strip.Color(0, 0, 255), 100);
  if (isTrigger() || magicModePressed()) {
    return;
  }
  
  delay(random(100, 200));
  if (isTrigger() || magicModePressed()) {
    return;
  }
  
  flash(strip.Color(0, 0, 255), 100);
  if (isTrigger() || magicModePressed()) {
    return;
  }
  delay(random(100, 200));
  if (isTrigger() || magicModePressed()) {
    return;
  }
  flash(strip.Color(0, 0, 255), 100);
  if (isTrigger() || magicModePressed()) {
    return;
  }
  delay(random(100, 200));
}


void fireTest() {
  uint8_t fudgeHeight;
  int fudgeRatio = strip.numPixels() / 5;
  
  for (fudgeHeight = fudgeRatio; fudgeHeight < strip.numPixels() - fudgeRatio; fudgeHeight++) {
    delay(0);
    fire(fudgeHeight, fudgeHeight + fudgeRatio, 10, 100);
    delay(0);
    if (isTrigger() || magicModePressed()) {
      return;
    }
  }
  
  for (;fudgeHeight > 0; fudgeHeight--){
    delay(0);
    fire(fudgeHeight, fudgeHeight + fudgeRatio, 10, 100);
    delay(0);    
    if (isTrigger() || magicModePressed()) {
      return;
    }
  }
}

void arcaneTest() {
  uint8_t colorFudge, minFudge, maxFudge, waitMin, waitMax;
  
  waitMin = 150;
  waitMax = 300;
  minFudge = strip.numPixels() / 5;
  maxFudge = strip.numPixels() * 4 / 5;
  
  twistHelper(minFudge, maxFudge, waitMin, waitMax, 1);
  twistHelper(maxFudge, minFudge, waitMin, waitMax, -1);  
}


void nature() {
  // TODO;
}

void ice() {
  // draw thatch icicles
}

void earth(){ 
  // Draw vines
  strip.clear(); 
  vineHelper(strip.Color(0, 102, 0), 0);
  if (isTrigger() || magicModePressed()) {
      return;
  }
  #ifndef NOMOTION
  motionLoop();
  #endif
  vineHelper(strip.Color(0, 127, 0), 1);
  if (isTrigger() || magicModePressed()) {
    return;
  }
  #ifndef NOMOTION
  motionLoop();
  #endif
  vineHelper(strip.Color(128, 255, 0), 2);
  strip.clear(); 
}

void swirlLinesTest(){
  uint8_t colorFudge;
  
  for (colorFudge=0; colorFudge < 255 && mode == 0; colorFudge++) {
    //handleWifiRequest();
    swirlLines(
      wheel(colorFudge),       // Color 1
      wheel(colorFudge + 8),  // Color 2
      wheel(colorFudge + 16),  // Color 3
      wheel(colorFudge + 24),  // Color 4
      10);                     // Delay
    //handleWifiRequest();
    if (isTrigger() || magicModePressed()) {
      return;
    }
  }
}

int accumDrops = 0;
int accumSeedRate = 50;
int accumLimit = strip.numPixels() / 2;
int dropSpeed = 20;
int accumRate = 5;
int pos_drop = 0;
void trickle(float angle_x, float angle_y, float angle_z) {
  //dropSpeed = abs(angle_x) / 10;

  strip.clear();
  int j=0;
  if (dir_trickle_up) {
    pos_drop += dropSpeed;
    for (j=0; j < accumRate + random(5); j++){
      strip.setPixelColor(pos_drop + j, strip.Color(0,0,207));
    }
    for (int i=strip.numPixels(); i > strip.numPixels() - (accumDrops + random(accumSeedRate)); i--){         
      strip.setPixelColor(i, strip.Color(0,0,207));
    }
  } else {
    pos_drop -= dropSpeed;
    for (j=0; j < accumRate + random(5); j++){
      strip.setPixelColor(pos_drop + j, strip.Color(0,0,207));
    }
    for (int i=0; i < (accumDrops + random(accumSeedRate)); i++){         
      strip.setPixelColor(i, strip.Color(0,0,207));
    }
  }
  
  if (pos_drop >= strip.numPixels() || pos_drop <= 0){
    pos_drop = dir_trickle_up ? 0 : strip.numPixels();
    accumDrops += accumRate;
  }
  
  if (accumDrops > accumLimit){
    accumDrops = 0;
  }

  strip.show();
}

// Draws lines for calibration
void motionCalibration (float angle_x, float angle_y, float angle_z) {
  strip.clear();
  
  // Split up each section into 3rds
  int calc = (strip.numPixels() / 3); 
  
  int mod_x = ((angle_x + 180) / 360) * calc;
  int mod_y = (((angle_y + 180) / 360) * calc) + calc;
  int mod_z = (((angle_z + 180) / 360) * calc) + (calc*2);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (i < mod_x){
      strip.setPixelColor(i, strip.Color(200,0,0));
    }
    else if (i < mod_y && i > calc){
      strip.setPixelColor(i, strip.Color(0,200,0));
    }
    else if (i < mod_z && i > (calc*2)){
      strip.setPixelColor(i, strip.Color(0,0,200));
    }
  }
  strip.show();
}
#endif

