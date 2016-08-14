/**
 * gps.h - All GPS related functions
 */

#include <cmath> 
#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0

// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / pi);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}


int compassThreshold = 2;
bool compassInTarget(int target) {
  int difference = compassDir - target;
  
  if (difference < 0) { difference *= -1; }

  if (difference < compassThreshold) {
    return true;    
  }
  return false;
}

void compass() {
  for (int i=0; i < strip.numPixels(); i++) {
    if (compassInTarget(0 + i - (strip.numPixels() / 2))) { // North
      strip.setPixelColor(i, 255, 0, 0);
    } else if (compassInTarget(90 + i - (strip.numPixels() / 2))) { // West
      strip.setPixelColor(i, 0, 255, 0);
    } else if (compassInTarget(180 + i - (strip.numPixels() / 2))) { // South
      strip.setPixelColor(i, 255, 255, 255);
    } else if (compassInTarget(270 + i - (strip.numPixels() / 2))) { // East
      strip.setPixelColor(i, 0, 0, 255);
    } else {
      strip.setPixelColor(i, 0, 0, 0);  
    }
  }
  strip.show();
  delay(20);
}

/**
 * Calculates the distance to the stored waypoint and changes colors accordingly.
 */
void distanceHome() {
  // How about some love for Haversine
  double distanceKmeters = distanceEarth(latitude, longitude, currLat, currLong);
  
  char distanceStr[64];
  dtostrf(distanceKmeters, 6, 15, distanceStr);

  // Useful for debugging.
  //Serial.print("Distance (km): "); Serial.println(String(distanceStr));

  int meters = distanceKmeters * 1000;

  uint32_t gpsColor = strip.Color(255, 0, 0);
  int minDist = 1000;
  int maxDist = 2000;
  if (meters < 25) {
    gpsColor = strip.Color(0, 255, 0); // Green, close    
    // TODO: somehow use all 25 LEDs
    minDist = 0;
    maxDist = 25;
  } else if (meters  < 100) {
    gpsColor = strip.Color(255, 255, 0); // Yellow, less close
    minDist = 25, maxDist = 100;
  } else if (meters  < 1000) {
    gpsColor = strip.Color(255, 20, 147); // Pink, even less close
    minDist = 100, maxDist = 1000;
  } // > 1 KM, you are fucked


  int range = maxDist - minDist;
  int percent = ((meters - minDist) * 100) / range;
  Serial.print("Dist Percent: "); Serial.println(percent);

  int numLedsToDraw = strip.numPixels() * (percent / 100.0f);

  // Not so bright, guys
  strip.clear();
  for (int i=0; i < numLedsToDraw / 2 ; i++){
    strip.setPixelColor(i, gpsColor);
  }
  strip.show();
  
}

void findHome() {
  // TODO: math
  double diffLong = currLong - longitude;
  double diffLat = currLat - latitude;

  // debugging
  char latitudeStr[64];
  char longitudeStr[64];
  dtostrf(diffLat, 6, 15, latitudeStr);
  dtostrf(diffLong, 6, 15, longitudeStr);
  Serial.print("Lat Diff: "); Serial.println(latitudeStr);
  Serial.print("Long Diff: "); Serial.println(longitudeStr);
  
  for (int i=0; i < strip.numPixels(); i++) {
    if (compassInTarget(0 + i - (strip.numPixels() / 2))) { // North
      strip.setPixelColor(i, 255, 0, 0);
    } else if (compassInTarget(90 + i - (strip.numPixels() / 2))) { // West
      strip.setPixelColor(i, 0, 255, 0);
    } else if (compassInTarget(180 + i - (strip.numPixels() / 2))) { // South
      strip.setPixelColor(i, 255, 255, 255);
    } else if (compassInTarget(270 + i - (strip.numPixels() / 2))) { // East
      strip.setPixelColor(i, 0, 0, 255);
    } else {
      strip.setPixelColor(i, 0, 0, 0);  
    }
  }
  strip.show();
  delay(20);
}
