/**
 * settings.h - Settings menus and related globals.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

// ONLY COMMMENT OUT FOR BATTERY TESTING
//#define BURNINTEST

/**
 * updateSelection - Helper that rotates the current menu choice.
 */
int settingsPressCount = 0;
unsigned long lastPress = 1;
#define SWITCHWAITPERIOD 200
int updateSelection(int numModes) {
  float buttonState = digitalRead(MODE_PIN);

  int difference = millis() - lastPress;
  if (buttonState != HIGH) {
    hasNotification = false;
    if (difference < SWITCHWAITPERIOD) {
      settingsPressCount += 1;
    } else {
      settingsPressCount -= 1;
    }
    while(buttonState != HIGH) {
        buttonState = digitalRead(MODE_PIN);
    }

    if (settingsPressCount < 1){
      settingsPressCount = 1;
    }
    if (settingsPressCount > numModes){
      settingsPressCount = numModes;
    }
    
    lastPress = millis();
    return settingsPressCount;
  } else {
    if (settingsPressCount > 0 && difference < SWITCHWAITPERIOD) {
      // You could reset counter here.
    }
  }
  return 0;
}


/**
 * displaySettings - Renders the currently selected mode to the LED strip.
 * 
 * @param selectedMode The current mode selection to draw.
 */
void displaySettings(int selectedMode) {
  int mod = strip.numPixels() / 4;
  for (int i=0; i < strip.numPixels(); i++){
    if (i < mod){
      // Green - size
      if (i <= selectedMode) // Yellow - brightness
        strip.setPixelColor(i, strip.Color(255, 255, 0));
      else
        strip.setPixelColor(i, 0);
    } else if (i < (mod) * 2){ // green - size
      if ((i % mod) <= selectedMode)
        strip.setPixelColor(i, strip.Color(0, 255, 0));
      else
        strip.setPixelColor(i, 0);
    } else if (i < (mod) * 3){ // blue - exit, the story ends
      if ((i % mod) <= selectedMode)
        strip.setPixelColor(i, strip.Color(0, 0, 255));
      else
        strip.setPixelColor(i, 0);
    } else { // red - how deep does the rabbit hole go?
      if ((i % mod) <= selectedMode)
        strip.setPixelColor(i, strip.Color(255, 0, 0));
       else
        strip.setPixelColor(i, 0);
    }
  }
  strip.show();
}

/**
 * displayMode - Renders the display configuration UI to LEDs.
 * 
 * @param settingsMode The currently selected value.
 */
void displayMode(int settingsMode) {
  int mod = strip.numPixels() / 4;
  uint32_t color = 0xFF0000;
  switch(settingsMode) {
    case 1:
      color = strip.Color(0, 255, 0);
      break;
    case 2:
      color = strip.Color(255, 255, 0);
      break;  
    case 3:
      color = strip.Color(255, 0, 0);
      break;
    case 4:
      color = strip.Color(0, 0, 255);
      break;
  }
  for (int i = 0; i < strip.numPixels(); i++){
    if ((mod * (settingsMode - 1)) <= i && (mod * settingsMode) > i) {
      strip.setPixelColor(i, color);
    } else {
      strip.setPixelColor(i, 0);  
    }
  }
  strip.show();
}

/**
 * settingsSelect - Button press code for settings.
 */
#define MODESELECTWAIT 1500
int settingsSelect(){
  // Lazily wait for press
  unsigned long settingsPressTime = millis();
  int settingsDifference = 0;
  int selectedOption = 0;
  int lastInput = 0;
  settingsPressCount = 1;
  while (settingsDifference < MODESELECTWAIT) {
    settingsDifference = millis() - settingsPressTime;
    lastInput = updateSelection(4);
    if (lastInput > 0){
      selectedOption = lastInput;
      settingsPressTime = millis();
      displayMode(selectedOption);
    }
    delay(0);
  }
  return selectedOption;
}

/**
 * displayBrightness - Renders the display configuration UI to LEDs.
 * 
 * @param currentBrightness The currently selected value [1...5].
 */
void displayBrightness(int currentBrightness) {
  int limit = currentBrightness * (strip.numPixels() / 5);
  for (int i=0; i < strip.numPixels(); i++) {
    if (i < limit) {
      strip.setPixelColor(i, wheel(25)); // purple
    } else {
      strip.setPixelColor(i, 0);
    }
  } 
  strip.show();
}

/**
 * brightnessSelect - Entry point for brightness selection code, renders
 *    UI and responds to button presses accordingly.
 */
void brightnessSelect() {
  // Lazily wait for press
  unsigned long settingsPressTime = millis();
  int settingsDifference = 0;
  int selectedOption = 0;
  int lastInput = 1;
  settingsPressCount = 0;
  displayBrightness(3);
  delay(250);
  while (settingsDifference < MODESELECTWAIT) {
    settingsDifference = millis() - settingsPressTime;
    lastInput = updateSelection(5);
    Serial.println(lastInput);
    if (lastInput > 0){
      selectedOption = lastInput;
      settingsPressTime = millis();
      displayBrightness(selectedOption);
    }
    delay(0);
  }
  if (selectedOption > 0){
    strip.setBrightness(selectedOption * 50);
    FastLED.setBrightness( selectedOption * 50 );
    
  }
}

/**
 * updateSize - Renders current selection for strip size.
 * 
 * @param currentSize The current size, in pixels of the LED strip.
 */
void updateSize(int currentSize) {
  strip.updateLength(currentSize * 5);
  
  for (int i=0; i < strip.numPixels(); i++) {
    if (i == 0 || i == strip.numPixels() - 1) {
      strip.setPixelColor(i, 0xFFFFFF); // white ends
    } else {
      strip.setPixelColor(i, 0);
    }
  } 
  strip.show();

  
  NUM_LEDS = currentSize * 5;
  fastLedSetup();
}

/**
 * sizeSelect - Code for selecting the size of the LED strip.
 */
// TODO: refactorme
void sizeSelect() {
  // Lazily wait for press
  unsigned long settingsPressTime = millis();
  int settingsDifference = 0;
  int selectedOption = 0;
  int lastInput = 1;
  settingsPressCount = 0;
  updateSize(5);
  delay(250);
  while (settingsDifference < MODESELECTWAIT) {
    settingsDifference = millis() - settingsPressTime;
    lastInput = updateSelection(50);
    Serial.println(lastInput);
    if (lastInput > 0){
      selectedOption = lastInput;
      settingsPressTime = millis();
      updateSize(selectedOption);
    }
    delay(0);
  }
}

// forward declaration:
void controlLoop();

/**
 * settings() - Entry point for the settings menu.
 */
void settings() {
  for (int i=1; i < 5; i++) {
    displayMode(i);
    delay(200);
  }

  int selectedItem = settingsSelect();
  Serial.print("Selected: ");
  Serial.println(selectedItem);
  switch (selectedItem) {
    case 1:
      brightnessSelect();
      break;
    case 2:
      sizeSelect();
      break;
    case 3:
      // Red pill - Exit to control loop menu.
      controlLoop();
      break;
    default:
      // Do nothing
      break;
  }
}
#endif
