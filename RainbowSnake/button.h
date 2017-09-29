// Button.h - All primary button functions are here.
//            settings button functions are in settings.h
#ifndef BUTTON_H
#define BUTTON_H

#ifndef USEPREFS
#include <EEPROM.h>
#endif

#ifndef NOT_FAST 
boolean notFast = false;
#endif
#ifdef NOT_FAST
boolean notFast = true; // disable fastled patterns
#endif

void changeMode() {
  hasNotification = false;
  if (notFast){
    Serial.println("Not fast");
    if (mode > numModes){
      mode = 0;
    } else {
      mode++;
    }
  } else {
    // Careful, this is correct AFAIK
    if (mode >= numFastModes){
      mode = 0;
    } else {
      mode++;
    }
  }
  
  // Skip sparkle aka ping mode.
  if (mode == 2){
    mode++;
  }

  // Store the current mode in EEPROM
  #ifndef USEPREFS
  EEPROM.write(1, mode);
  EEPROM.commit();
  #endif
}

void onePress(int count) {
    for (int i=0; i < strip.numPixels(); i++){
    if (i < count){
      // Same same, but different
      //strip.setPixelColor(i, wheel(count*(255/strip.numPixels())));
      // Rainbow love
      strip.setPixelColor(i, wheel(i*(255/strip.numPixels())));
    }
    if ((strip.numPixels() - count) < (strip.numPixels() - i)) {
      strip.setPixelColor(strip.numPixels() - (i+1), wheel(i*(255/strip.numPixels())));
    } else {
      if (i >= count && i < (strip.numPixels() / 2)) {
        strip.setPixelColor(i, 0);
      } else {
        if (i >= (strip.numPixels() / 2) && (strip.numPixels() - count) >= (strip.numPixels() - i) ){
          //strip.setPixelColor(strip.numPixels() - (i+1), 0);
        }
      }
    }
  }
  strip.show();
  delay(50);
}

void twoPress(int count) {
    for (int i=0; i < strip.numPixels(); i++){
    if (i < count){
      strip.setPixelColor(i, 0xFF0000);
    }
    if ((strip.numPixels() - count) < (strip.numPixels() - i)) {
      strip.setPixelColor(strip.numPixels() - (i+1), 0x00FF00);
    } else {
      if (i >= count && i < (strip.numPixels() / 2)) {
        strip.setPixelColor(i, 0);
      } else {
        if (i >= (strip.numPixels() / 2) && (strip.numPixels() - count) >= (strip.numPixels() - i) ){
          //strip.setPixelColor(strip.numPixels() - (i+1), 0);
        }
      }
    }
  }
  strip.show();
  delay(50);
}

void threePress(int count) {
    for (int i=0; i < strip.numPixels(); i++){
    if (i < count){
      strip.setPixelColor(i, 0x000FF);
    }
    if ((strip.numPixels() - count) < (strip.numPixels() - i)) {
      strip.setPixelColor(strip.numPixels() - (i+1), 0xFF0000);
    } else {
      if (i >= count && i < (strip.numPixels() / 2)) {
        strip.setPixelColor(i, 0);
      } else {
        if (i >= (strip.numPixels() / 2) && (strip.numPixels() - count) >= (strip.numPixels() - i) ){
          //strip.setPixelColor(strip.numPixels() - (i+1), 0);
        }
      }
    }
  }
  strip.show();
  delay(50);
}

unsigned long lastPresses[] = {0, 700, 1400};
int pressCounter = 0;
int pressCounts = 3; // used in modulo
#ifndef NOWIFI
String sparkleRequest = String("POST /pattern?value=") + String(mode) + 
    String(" HTTP/1.1\r\n") + String("Host: ") + String(SERVER_NAME) + 
    String("\r\n") + String("Connection: close\r\n\r\n");
#endif
#ifdef NOWIFI
String sparkleRequest = "";
#endif

void checkModeButton() {
    float buttonState = digitalRead(MODE_PIN);

    // check if the pushbutton is pressed.
    // if it is, the buttonState is LOW:
    if (buttonState != HIGH) {
      hasNotification = false;
      Serial.println("Button triggered.");

      unsigned long milliStart = millis();
      lastPresses[(pressCounter % pressCounts)] = milliStart;  
      ++pressCounter;

      int buttonCtr = 0;
      for (int i=0; i < pressCounts; i++) {
        unsigned long curr = lastPresses[((pressCounter + i) % pressCounts)];
        unsigned long last = lastPresses[((pressCounter + i - 1) % pressCounts)];
        if ((curr - last) < 600) {
          buttonCtr++;
          // Easier to click, approximates a bpm
          //gBpm = 200 - ((curr - last) / 3) ;
          // Actual bpm calculation
          gBpm = (1000 /(curr - last)) * 60;
        }
      }
      // buttonCtr now has the press counts


      // Block until the button is unpressed
      int count = 0;
      int delayPerLeds = 50;
      
      while (buttonState != HIGH) {
        buttonState = digitalRead(MODE_PIN);
        switch (buttonCtr) {
          case 1:
            twoPress(count++);
            break;
          case 2:
            threePress(count++);
            break;
          default:
            onePress(count++);
            break;
        }  
      }
      
      Serial.print("Elapsed: ");
      unsigned long elapsed = millis() - milliStart;
      Serial.println(elapsed);

      // TODO: return enum.
      // Show brightness after 5 secs.
      if (elapsed > 1500) {
        for (int i=0; i < pressCounts; i++) {
          lastPresses[i] = i*10000;
        }
        settings();
        delay(1000);
        return;
      } else if (elapsed > 500){
        for (int i=0; i < pressCounts; i++) {
          lastPresses[i] = i*10000;
        }
        
        switch(buttonCtr){ 
          case 2:
            updateMeshMode();
            break;
          case 1:
            peerCount = 0;
            sendMessageManual(sparkleRequest);        
            break;
          default:
            changeMode();
            break;
        }

      } else {
        // UI update...
        checkModeButton();
      }
    }
}
#endif
