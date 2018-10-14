#define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
#define NOISE     10  // Noise/hum/interference in mic signal
#define SAMPLES   60  // Length of buffer for dynamic level adjustment
#define TOP       (NUM_LEDS + 2) // Allow dot to go slightly off scale
#define PEAK_FALL 40  // Rate of peak falling dot
 
void setupVU() {
  //analogReference(EXTERNAL);
  //pinMode(MIC_PIN, INPUT);
  //memset(vol, 0, sizeof(vol));
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
//Used to draw a line between two points of a given color
void drawLine(uint8_t from, uint8_t to, CRGB c) {
  uint8_t fromTemp;
  if (from > to) {
    fromTemp = from;
    from = to;
    to = fromTemp;
  }
  for(int i=from; i<=to; i++){
    leds[i] = c;
  }
}
 
 
float fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve){
 
  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;
 
 
  // condition curve parameter
  // limit range
 
  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;
 
  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output 
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function
 
  /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
   Serial.println(); 
   */
 
  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }
 
  // Zero Refference the values
  OriginalRange = originalMax - originalMin;
 
  if (newEnd > newBegin){ 
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd; 
    invFlag = 1;
  }
 
  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float
 
  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine 
  if (originalMin > originalMax ) {
    return 0;
  }
 
  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;
 
  }
  else     // invert the ranges
  {   
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange); 
  }
 
  return rangedValue;
}

#define SAMPLE_WINDOW   10  // Sample window for average level
#define PEAK_HANG 24 //Time of pause before peak dot falls
#define PEAK_FALL 4 //Rate of falling peak dot
#define INPUT_FLOOR 10 //Lower range of analogRead input
#define INPUT_CEILING 300 //Max range of analogRead input, the lower the value the more sensitive (1023 = max)
byte peak = 16;      // Peak level of column; used for falling dots
unsigned int sample;
byte dotCount = 0;  //Frame counter for peak dot
byte dotHangCount = 0; //Frame counter for holding peak dot

void vuLoop() {
  unsigned long startMillis= millis();  // Start of sample window
  float peakToPeak = 0;   // peak-to-peak level
 
  unsigned int signalMax = 0;
  unsigned int signalMin = 1023;
  unsigned int c, y;
 
 
  // collect data for length of sample window (in mS)
  while (millis() - startMillis < SAMPLE_WINDOW) {
    sample = analogRead(MIC_PIN) / 3; // Magic Fudge
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
 
  // Serial.println(peakToPeak);
 
 
  //Fill the strip with rainbow gradient
  byte low, high = 0;
  switch (oneMod32 % 4) {
    case 0:
      low = 30;
      high = 150;
      break;
    case 2:
      low = 0;
      high = 85;
      break;
    case 3:
      low = 128;
      high = 248;
      break;
    case 4:
      low = 80;
      high = 200;
      break;      
    default:
      low = 30;
      high = 150;
      break;      
  }
  
  for (int i=0; i <= NUM_LEDS-1; i++){
    leds[i] = Wheel(map(i, 0, NUM_LEDS-1, low, high));
  }
 
  //Scale the input logarithmically instead of linearly
  c = fscale(INPUT_FLOOR, INPUT_CEILING, NUM_LEDS, 0, peakToPeak, 2);
 
  if(c < peak) {
    peak = c;        // Keep dot on top
    dotHangCount = 0;    // make the dot hang before falling
  }
  if (c <= NUM_LEDS) { // Fill partial column with off pixels
    drawLine(NUM_LEDS, NUM_LEDS-c, 0);
  }
 
  // Set the peak dot to match the rainbow gradient
  y = NUM_LEDS - peak;
  
  leds[y-1] = Wheel(map(y, 0, NUM_LEDS-1, 30, 150));
 
  FastLED.show();
 
  // Frame based peak dot animation
  if(dotHangCount > PEAK_HANG) { //Peak pause length
    if(++dotCount >= PEAK_FALL) { //Fall rate 
      peak++;
      dotCount = 0;
    }
  } else {
    dotHangCount++; 
  }
}



// Arduino Beat Detector By Damian Peckett 2015
// License: Public Domain.

// Our Global Sample Rate, 5000hz
#define SAMPLEPERIODUS 200

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// 20 - 200hz Single Pole Bandpass IIR Filter
float bassFilter(float sample) {
    static float xv[3] = {0,0,0}, yv[3] = {0,0,0};
    xv[0] = xv[1]; xv[1] = xv[2]; 
    xv[2] = (sample) / 3.f; // change here to values close to 2, to adapt for stronger or weeker sources of line level audio  
    

    yv[0] = yv[1]; yv[1] = yv[2]; 
    yv[2] = (xv[2] - xv[0])
        + (-0.7960060012f * yv[0]) + (1.7903124146f * yv[1]);
    return yv[2];
}

// 10hz Single Pole Lowpass IIR Filter
float envelopeFilter(float sample) { //10hz low pass
    static float xv[2] = {0,0}, yv[2] = {0,0};
    xv[0] = xv[1]; 
    xv[1] = sample / 50.f;
    yv[0] = yv[1]; 
    yv[1] = (xv[0] + xv[1]) + (0.9875119299f * yv[0]);
    return yv[1];
}

// 1.7 - 3.0hz Single Pole Bandpass IIR Filter
float beatFilter(float sample) {
    static float xv[3] = {0,0,0}, yv[3] = {0,0,0};
    xv[0] = xv[1]; xv[1] = xv[2]; 
    xv[2] = sample / 2.7f;
    yv[0] = yv[1]; yv[1] = yv[2]; 
    yv[2] = (xv[2] - xv[0])
        + (-0.7169861741f * yv[0]) + (1.4453653501f * yv[1]);
    return yv[2];
}

// Draw the up / down angle of the beat
void walkBeats(boolean isBeat, float slope) {  
  static int hueCtr = 0;

  for (int i=NUM_LEDS-1; i > 0; i--) {
    leds[i] = leds[i-1];
  }
  
  if (isBeat) {
    //leds[0] = Wheel(hueCtr++ % 255);
    leds[0] = CHSV(hueCtr++ % 255, 255, 150 + (slope * 10));
  } else {
    leds[0] = 0;
  }
  FastLED.show();
}

void processBeat(float beat) {
  // If we are above threshold, light up LED
  if(beat > 0) {
    walkBeats(true, beat);
  } else {
    walkBeats(false, beat);
  }

#define DEBUGBEATS
#ifdef DEBUGBEATS
  Serial.println(beat);
#endif
}

void beatDetectLoop() {
    unsigned long time = micros(); // Used to track rate
    static float sample, value, envelope, beat, thresh;
    static char beatI;
    
    // Read ADC and center so +-512
    sample = (float)analogRead(MIC_PIN);

    // Filter only bass component
    value = bassFilter(sample);

    // Take signal amplitude and filter
    if(value < 0)value=-value;
    envelope = envelopeFilter(value);

    // Every 200 samples (25hz) filter the envelope 
    if(beatI++ >= 100) {
      // Filter out repeating bass sounds 100 - 180bpm
      beat = beatFilter(envelope);
      processBeat(beat);      

      //Reset sample counter
      beatI = 0;
    }
    if (beatI++ % 50 == 0) {
      walkBeats(0, false);
    }
}
  

/*
byte
  peak      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
  volCount  = 0;      // Frame counter for storing past volume data
int
  vol[SAMPLES],       // Collection of prior volume samples
  lvl       = 10,      // Current "dampened" audio level
  minLvlAvg = 0,      // For dynamic adjustment of graph low & high
  maxLvlAvg = 512;
void vuLoopAdj() {
  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;
  
  n   = analogRead(MIC_PIN); // Raw reading from mic 
  n   = abs(n - 512 - DC_OFFSET); // Center on zero
  n   = (n <= NOISE) ? 0 : (n - NOISE);             // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)
 
  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);
 
  if(height < 0L)       height = 0;      // Clip output
  else if(height > TOP) height = TOP;
  if(height > peak)     peak   = height; // Keep 'peak' dot at top
 
 
  // Color pixels based on rainbow gradient
  for(i=0; i<NUM_LEDS; i++) {
    if(i >= height) leds[i] = 0;
    else leds[i] = Wheel(map(i, 0, NUM_LEDS - 1, 30, 150));
  }

  // Draw peak dot  
  if(peak > 0 && peak <= NUM_LEDS-1) leds[peak] = Wheel(map(peak,0,NUM_LEDS-1,30,150));
  
  FastLED.show(); // Update strip
 
  // Every few frames, make the peak pixel drop by 1: 
  if(++dotCount >= PEAK_FALL) { //fall rate 
    
    if(peak > 0) peak--;
    dotCount = 0;
  }
 
  vol[volCount] = n;                      // Save sample for dynamic leveling
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter
 
  // Get volume range of prior frames
  minLvl = maxLvl = vol[0];
  for(i=1; i<SAMPLES; i++) {
    if(vol[i] < minLvl) minLvl = vol[i];
    else if(vol[i] > maxLvl) maxLvl = vol[i];
  }
  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)
}*/
