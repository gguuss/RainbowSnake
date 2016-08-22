int povTick = 0;
int poiMode = 0;

// Notes about poi spinning
//
// typical

void poiTwoColor(uint32_t color1, uint32_t color2) {
  uint32_t poiColor = color1;
  int pos = beatsin16(gBpm, 0, 8);
  if (pos > 3) {
    poiColor = color2;
  }

  colorWipe(poiColor, 0);
}

void poiLoop() {
  povTick++;
  switch (poiMode) {
    case 0:
      poiTwoColor(poiColor1, poiColor2);
      break;
    default:
      break;
  }
  // Because ESP
  delay(0);
}

