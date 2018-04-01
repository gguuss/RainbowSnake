//
// This header encapsulates the LED rotary switch / button hardware.
//

// RGB LED colors (for common anode LED, 0 is on, 1 is off)
#define OFF B111
#define RED B110
#define GREEN B101
#define YELLOW B100
#define BLUE B011
#define PURPLE B010
#define CYAN B001
#define WHITE B000

volatile int rotary_counter = 0; // current "position" of rotary encoder (increments CW) 
volatile boolean rotary_change = false; // will turn true if rotary_counter has changed
volatile boolean button_pressed = false; // will turn true if the button has been pushed
volatile boolean button_released = false; // will turn true if the button has been released (sets button_downtime)
volatile unsigned long button_downtime = 0L; // ms the button was pushed before release

void pollRotButton() {
  #ifdef ROT_SW
  static boolean bncState = false;
  static unsigned long bncStart, bncEnd;

  // Debounce button
  if ((digitalRead(ROT_SW) == HIGH) && (bncState == false)) {
    bncStart = millis();
    if (bncStart > (bncEnd + 10)) { // 10ms debounce timer
      bncState = true;
      button_pressed = true;
    }    
  } else if ((digitalRead(ROT_SW) == LOW) && (bncState == true)) {    
    bncEnd = millis();
    if (bncEnd > (bncStart + 10)) {
      bncState = false;
      button_released = true;
      button_downtime = bncEnd - bncStart;
    }
  }
  #endif
}

void pollRotary() {
  #ifdef ROT_A
  static unsigned char rotary_state = 0; // current and previous encoder states

  rotary_state <<= 2;  // remember previous state
  rotary_state |= (digitalRead(ROT_A) | (digitalRead(ROT_B) << 1));  // mask in current state
  rotary_state &= 0x0F; // zero upper nybble
  if (rotary_state == 0x09) { // from 10 to 01, increment counter. Also try 0x06 if unreliable
    rotary_counter++;
    rotary_change = true;
  } else if (rotary_state == 0x03) { //decrement counter
    rotary_counter--;
    rotary_change = true;
  }
  #endif
}

void setLED(unsigned char color) {
  #ifdef ROT_LEDR
  digitalWrite(ROT_LEDR,color & B001);
  digitalWrite(ROT_LEDG,color & B010);
  digitalWrite(ROT_LEDB,color & B100);  
  #endif
}

void updateLED(int state) {
  unsigned char col = OFF;
  switch(state) {
    case 0:
      col = RED;
      break;
    case 1:
      col = GREEN;
      break;
    case 2:
      col = YELLOW;
      break;
    case 3:
      col = BLUE;
      break;
    case 4:
      col = PURPLE;
      break;
    case 5:
      col = CYAN;
      break;
    case 6:
      col = WHITE;
      break;
    default:
      break;
  }
  setLED(col);
}

long oldPosition  = -999;
void rotaryLoop() {
  #ifdef ROT_A
  static unsigned char x = 0;
  static boolean button_down = false;
  static unsigned long int button_down_start, button_down_time;

  static int counter = 0;  
  
  pollRotButton();
  pollRotary();
  
  counter++;

  if (rotary_change) {
    Serial.print("rotary: ");
    Serial.println(rotary_counter,DEC);
    int mod = rotary_counter;
    if (mod < 0) mod *= -1;
    updateLED(mod % 8);
    mode = abs(round(mod % 11));
    rotary_change = false; // Clear flag
  }

  if (button_pressed) {
    Serial.println("button press");
    x++; setLED(x); // Change the color of the knob LED
    button_pressed = false; // Clear flag
    button_down = true;
    button_down_start = millis();
  }

  if (button_released) {
    Serial.print("button release, downtime: ");
    Serial.println(button_downtime,DEC);
    x++; setLED(x); // Change the color of the knob LED
    button_released = false; // Clear flag
    button_down = false;
  }

  if (button_down) {
    button_down_time = millis() - button_down_start;
    if (button_down_time > 1000)
      Serial.println("button held down for one second");
  }
  #endif
}

void setupRotary() {
  #ifdef ROT_A
  // Set up all the I/O pins. Unused pins are commented out.
  pinMode(ROT_B, INPUT);
  digitalWrite(ROT_B, HIGH); // turn on weak pullup
  pinMode(ROT_A, INPUT);
  digitalWrite(ROT_A, HIGH); // turn on weak pullup
  pinMode(ROT_SW, INPUT);
  digitalWrite(ROT_SW, LOW);

  pinMode(ROT_LEDB, OUTPUT);
  pinMode(ROT_LEDG, OUTPUT);
  pinMode(ROT_LEDR, OUTPUT);

  setLED(RED);
  #endif
}
