#ifndef CONTROL_H
#define CONTROL_H
/**
 * Control.h - For mmass control of LED mesh
 */

// For TESTING, don't mess with.
boolean skipAwful = true; // Disable bad patterns

int controlModes = notFast ? numModes : numFastModes;
void displayControlMode(int settingsMode) {
  int mod = strip.numPixels() / controlModes;
  for (int i = 0; i < strip.numPixels(); i++){
    uint32_t color = wheel((255/strip.numPixels()) * i);
    if ((mod * (settingsMode - 1)) <= i && (mod * settingsMode) > i) {
      strip.setPixelColor(i, color);
    } else {
      strip.setPixelColor(i, 0);  
    }
  }
  strip.show();
}


#define CONTROLSELECTWAIT 1500
int controlSelect(){
  // Lazily wait for press
  unsigned long settingsPressTime = millis();
  int settingsDifference = 0;
  int selectedOption = 0;
  int lastInput = 0;
  settingsPressCount = 1;
  while (settingsDifference < CONTROLSELECTWAIT) {
    settingsDifference = millis() - settingsPressTime;
    lastInput = updateSelection(controlModes);
    if (lastInput > 0){
      selectedOption = lastInput;
      settingsPressTime = millis();
      displayControlMode(selectedOption);
    }
    delay(0);
  }
  return selectedOption;
}

void controlLoop() {
  for (int i=1; i < controlModes; i++) {
    displayControlMode(i);
    delay(200);
  }

  int selectedItem = controlSelect();
  Serial.print("Selected: ");
  Serial.println(selectedItem);

  mode = selectedItem;
  updateMeshMode();
}
#endif
