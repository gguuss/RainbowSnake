/**
 * colors_button.h - Color functions incorporating the button.
 */

bool buttonOn() {
  // check if the pushbutton is pressed.
  // if it is, the buttonState is LOW:
  float buttonState = digitalRead(MODE_PIN);
  return (buttonState != HIGH);
}

// rotate colors
void rotateAdaPixels() {
  uint32_t tempColor = strip.getPixelColor(strip.numPixels() - 1);
  for (int i = strip.numPixels() - 2; i >= 0; i--){
    strip.setPixelColor(i+1, strip.getPixelColor(i));
  }
  strip.setPixelColor(strip.numPixels() - 1, tempColor);
}

boolean changeColor = false;
int clickerColorIndex = 0;
/**
 * buttonClicker - Scrolls colors based on button presses.
 */
void buttonClicker() {
  rotateAdaPixels();
  if (buttonOn()) {
    if (changeColor) {
      clickerColorIndex += random(10, 25);
      if (clickerColorIndex > 255) {
        clickerColorIndex = 0;
      }
      changeColor = false;
    }
    strip.setPixelColor(0, wheel(clickerColorIndex));
  } else {
    changeColor = true;
    strip.setPixelColor(0, 0);
  }
  strip.show();
  delay(20);
}

int buttonClickerSize = 0;
int wheelTick = 0;
/**
 * growClicker - Grows a rainbow while pressing, shrinks on release
 */
void growClicker () {
  if (buttonOn()) {
    buttonClickerSize++;
    if (buttonClickerSize > strip.numPixels()) {
      buttonClickerSize = strip.numPixels();
    }
  } else {
    buttonClickerSize--;
    if (buttonClickerSize < 0){
      buttonClickerSize = 0;
    }
  }
  for (int i=0; i < strip.numPixels() && i < buttonClickerSize; i++) {
    strip.setPixelColor(i, wheel(wheelTick + (i * ( 255 / strip.numPixels() )) ));
  }
  for (int i=buttonClickerSize; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
  delay(20);
  wheelTick += 5;
}

