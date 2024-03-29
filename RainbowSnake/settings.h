/**
 * settings.h - Settings menus and related globals.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

// ONLY COMMMENT OUT FOR BATTERY TESTING
//#define BURNINTEST

/**
 * updateSelection - Helper that rotates the current menu choice.
 * 
 * @return 0 when timeout expires, otherwise 1 ... numModes.
 */
int settingsPressCount = 0;
unsigned long lastPress = 1;
#define SWITCHWAITPERIOD 200
int updateSelection(int numModes) {
  float buttonState = digitalRead(MODE_PIN);

  int difference = millis() - lastPress;
  if (buttonState != HIGH) {
    hasNotification = false;
    
    // Increment / decrement based on wait time
    if (difference < SWITCHWAITPERIOD) {
      settingsPressCount += 1;
    } else {
      settingsPressCount -= 1;
    }
    //
    while(buttonState != HIGH) {
        buttonState = digitalRead(MODE_PIN);
    }

    // Ensure we return 1 ... numModes
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
      if (i <= mod) // Yellow - brightness
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
  int mod = (strip.numPixels() / 4);
  uint32_t color = 0xFFFFFF;  
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

  // TODO: fix awful
  Serial.println("Mod : " + String(mod));
  Serial.println("settingsMode : " + String(settingsMode));

  strip.clear();
  for (int i = mod * (settingsMode - 1); i < mod * settingsMode && i < strip.numPixels(); i++){    
      strip.setPixelColor(i, color);
  }
  strip.show();
  delay(100);
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
  settingsPressCount = 0;
  while (settingsDifference < MODESELECTWAIT) {
    settingsDifference = millis() - settingsPressTime;
    lastInput = updateSelection(4);
    if (lastInput > 0){
      selectedOption = lastInput;
      settingsPressTime = millis();
      // TODO: fix awful, display does not correspond to input?
      Serial.println("input now: " + String(lastInput));      
    }
    delay(0);
    displayMode(selectedOption);
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

    // TODO: fix awful
    if (lastInput > 0){
      Serial.println(lastInput);
    }
    
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
    if (lastInput > 0) {
      Serial.println(lastInput);
    }
    if (lastInput > 0){
      selectedOption = lastInput;
      settingsPressTime = millis();
      updateSize(selectedOption);
    }
    delay(0);
  }
}

/**
 * Helpful animation indicating an option was selected.
 */
void animateMenuSelect(uint32_t color){
  // Position white pixels
  for (int i=0; i < 5; i++){
    strip.setPixelColor(i, color); 
    strip.show();
    delay(50);
    
    rotateAdaPixels();
  }
  // Clear pixels
  for (int i=0; i < strip.numPixels(); i++){
    rotateAdaPixels();
    strip.show();
    delay(25);
    
    strip.setPixelColor(0, 0);  
  }
}

// forward declaration:
void controlLoop();

/**
 * settings() - Entry point for the settings menu.
 */
void settings() {
  for (int i=1; i < 6; i++) {
    displayMode(i);
    delay(200);
  }

  int selectedItem = settingsSelect();
  Serial.print("Selected: ");
  Serial.println(selectedItem);
  switch (selectedItem) {
    case 1:
      animateMenuSelect(strip.Color(255, 255, 0)); // Yellow, brightness
      brightnessSelect();
      break;
    case 2:
      animateMenuSelect(strip.Color(255, 255, 0)); // Green, size
      sizeSelect(); 
      break;
    case 3:
      // Red pill - Exit to control loop menu.
      animateMenuSelect(strip.Color(255, 0, 0)); // Red, transmit specific mode
      controlLoop();
      break;
    default:
      animateMenuSelect(strip.Color(0, 0, 255)); // Blue, transmit specific mode
      break;
  }
}
#endif
