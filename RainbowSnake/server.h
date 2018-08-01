#ifndef SERVER_H
#define SERVER_H
/* server.h
 * All server properties and configuration forwarded to single page application.
 *
 * All code based on:
 * ESP8266 + FastLED + IR Remote + MSGEQ7: https://github.com/jasoncoon/esp8266-fastled-webserver
 * Copyright (C) 2015 Jason Coon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

FASTLED_USING_NAMESPACE

extern "C" {
#include "user_interface.h"
}
#include <stdlib.h>  // For strtod

#include <ESP8266WebServer.h>
#include <FS.h>
#include <EEPROM.h>

#include "command.h"
#include "GradientPalletes.h"
#include "colors.h"
#include "fastled.h"

String AP_NameString;
const bool apMode = true;
String message = "";
bool enableLoop = true; 

#include "mesh.h"

#define NETDEBUG

// AP mode password
const char WiFiAPPSK[] = "";

// Wi-Fi network to connect to (if not in AP mode)
const char* ssid = "";
const char* password = "";

ESP8266WebServer server(80);

uint8_t patternIndex = 0;
uint8_t currentPatternIndex = 0; // Index number of which pattern is current

CRGB solidColor = CRGB::Blue;
uint32_t poiColor1 = 0xFF0000;
uint32_t poiColor2 = 0xFFFF00;

uint8_t power = 1;

bool autoplayEnabled = false;
uint8_t autoPlayDurationSeconds = 10;
unsigned int autoPlayTimeout = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Current brightness settings

void sendPower() {
  String json = String("{'power':") + String(power) + String("}");
  server.send(200, "text/json", json);
  json = String();
}

void sendBrightness() {
  String json = String(brightness);
  server.send(200, "text/json", json);
  json = String();
}

void sendSolidColor() {
  String json = "{";
  json += "\"r\":" + String(solidColor.r);
  json += ",\"g\":" + String(solidColor.g);
  json += ",\"b\":" + String(solidColor.b);
  json += "}";
  server.send(200, "text/json", json);
  json = String();
}

void sendLatLong() {
  char latitudeStr[64];  
  char longitudeStr[64];  
  dtostrf(latitude, 6, 15, latitudeStr);
  dtostrf(longitude, 6, 15, longitudeStr);
  
  String json = String("{'lat':") + String(latitudeStr) + String(", 'long':") +
      String(longitudeStr) + "}";
  server.send(200, "text/json", json);
  json = String();
}

void setPower(uint8_t value) {
  if (value == 1) {
    FastLED.setBrightness(brightness);
    strip.setBrightness(brightness);
  } else {
    FastLED.setBrightness(0);
    strip.setBrightness(0);
  }
}

void showSolidColor() {
  fill_solid(leds, NUM_LEDS, solidColor);
  FastLED.show();
}


typedef void (*Pattern)();
typedef Pattern PatternList[];
typedef struct {
  Pattern pattern;
  String name;
} PatternAndName;
typedef PatternAndName PatternAndNameList[];

// List of patterns to cycle through.  Each is defined as a separate function below.
PatternAndNameList patterns = {
  { adaLoop, "Adafruit Color Loop" },
  { rainbowLove, "Rainbow Cylon" },
  { rainbow, "Rainbow" },
  { hsvLoop, "HSV Color loop" },
  { vLoop, "Color value cylon" },
  { rainbowAll, "Moving Rainbow" },
  { rainbowTheater, "Rainbow Theatre" },
  { pictureLoop, "Rainbow Theater Chase" },
  { fastRainbow, "Fast Rainbow" },
  { colorwaves, "Color Waves" },
  { palettetest, "Red Blue" },
  { fastRainbow, "Fire 2012" },
  { pride, "Rainbow" },
  { narwhal, "Narwhal" },
  { rainbowWithGlitter, "Rainbow With Glitter" },
  { confetti, "Confetti" },
  { juggle, "Fast Cylon" },
  { sinelon, "Sinelon" },
  { juggle, "Juggle" },
  { bounce, "Bounce" },
  { stackLights, "Stacks" },
  { stacking, "Stacking"},
  { stackingMore, "More Stacking" },
  { bpm, "BPM" },
  { pride, "Pride" },
  { buttonClicker, "Button Clicker" },
  { growClicker, "Grow Clicker" },
  { compass, "Compass" },
  { findHome, "Find Waypoint" },
  { countMeshNodes, "Count Mesh Nodes" },
  { drawVu, "VU Meter" },
  { drawVu, "Wizard Mode" },
  { drawVu, "POI" },
  { showSolidColor, "Show Solid Color" }
};
const uint8_t patternCount = ARRAY_SIZE(patterns);


void setPattern(int value) {
  // don't wrap around at the ends
  if (value < 0)
    value = 0;
  else if (value >= patternCount)
    value = patternCount - 1;

  currentPatternIndex = value;

  mode = value;

  EEPROM.write(1, currentPatternIndex);
  EEPROM.commit();
}

void setSolidColor(uint8_t r, uint8_t g, uint8_t b) {
  enableLoop = false;
  solidColor = CRGB(r, g, b);

  EEPROM.write(2, r);
  EEPROM.write(3, g);
  EEPROM.write(4, b);

  if (mode != POI) {
    setPattern(patternCount - 1);
  }
}


void setSolidColor(CRGB color)
{
  setSolidColor(color.r, color.g, color.b);
}

void sendPattern() {
  String json = "{";
  json += "\"index\":" + String(mode);
  json += ",\"name\":\"" + patterns[mode].name + "\"";
  json += "}";
  server.send(200, "text/json", json);
  json = String();
}

void saveLatLong() {
  EEPROM.put(6, latitude);
  EEPROM.put(6 + sizeof(double), longitude);
  EEPROM.commit();
}

void loadSettings()
{
  brightness = EEPROM.read(0);
  currentPatternIndex = EEPROM.read(1);
  if (currentPatternIndex < 0)
    currentPatternIndex = 0;
  else if (currentPatternIndex >= patternCount)
    currentPatternIndex = patternCount - 1;

  mode = currentPatternIndex;

  byte r = EEPROM.read(2);
  byte g = EEPROM.read(3);
  byte b = EEPROM.read(4);

  EEPROM.get(6, latitude);
  EEPROM.get(6 + sizeof(double), longitude);

  // FIXME: 
  // Loads size settings
  loadSize();

  if (r == 0 && g == 0 && b == 0)
  {
  }
  else
  {
    solidColor = CRGB(r, g, b);
  }
}

void sendMessage(){
  String json = "{";
  json += "\"message\":\"" + message + "\"";
  json += "}";
  server.send(200, "text/json", json);
  json = String();
  // Update peers with new message
  updateMeshMessage();
}


void sendAll() {
  String json = "{";

  json += "\"power\":" + String(power) + ",";
  json += "\"brightness\":" + String(brightness) + ",";

  json += "\"currentPattern\":{";
  json += "\"index\":" + String(mode);
  json += ",\"name\":\"" + patterns[mode].name + "\"}";

  json += ",\"message\":\"" + message + "\"";

  json += ",\"solidColor\":{";
  json += "\"r\":" + String(solidColor.r);
  json += ",\"g\":" + String(solidColor.g);
  json += ",\"b\":" + String(solidColor.b);
  json += "}";

  json += ",\"patterns\":[";
  for (uint8_t i = 0; i < patternCount; i++) {
    json += "\"" + patterns[i].name + "\"";
    if (i < patternCount - 1)
      json += ",";
  }
  json += "]";
  json += ",\"AP\": \"" + AP_NameString + "\"";
  json += "}";

  server.send(200, "text/json", json);
  json = String();
}


// increase or decrease the current pattern number, and wrap around at the ends
void adjustPattern(bool up) {
  if (up)
    currentPatternIndex++;
  else
    currentPatternIndex--;

  // wrap around at the ends
  if (currentPatternIndex < 0)
    currentPatternIndex = patternCount - 1;
  if (currentPatternIndex >= patternCount)
    currentPatternIndex = 0;

  EEPROM.write(1, currentPatternIndex);
  EEPROM.commit();
}

void setupServer(void) {
  EEPROM.begin(512);
  loadSettings();
  
  Serial.println();
  Serial.print( F("Heap: ") ); Serial.println(system_get_free_heap_size());
  Serial.print( F("Boot Vers: ") ); Serial.println(system_get_boot_version());
  Serial.print( F("CPU: ") ); Serial.println(system_get_cpu_freq());
  Serial.print( F("SDK: ") ); Serial.println(system_get_sdk_version());
  Serial.print( F("Chip ID: ") ); Serial.println(system_get_chip_id());
  Serial.print( F("Flash ID: ") ); Serial.println(spi_flash_get_id());
  Serial.print( F("Flash Size: ") ); Serial.println(ESP.getFlashChipRealSize());
  Serial.print( F("Vcc: ") ); Serial.println(ESP.getVcc());  
  Serial.println();

  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), String(fileSize).c_str());
    }
    Serial.printf("\n");
  }

  if (apMode) {
    WiFi.mode(WIFI_AP);

    // Do a little work to get a unique-ish name. Append the
    // last two bytes of the MAC (HEX'd) to "Thing-":
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);
    String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                   String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();
    AP_NameString = "Rainbow Snake " + macID;

    char AP_NameChar[AP_NameString.length() + 1];
    memset(AP_NameChar, 0, AP_NameString.length() + 1);

    for (int i = 0; i < AP_NameString.length(); i++)
      AP_NameChar[i] = AP_NameString.charAt(i);

    WiFi.softAP(AP_NameChar, WiFiAPPSK);

    Serial.printf("Connect to Wi-Fi access point: %s\n", AP_NameChar);
    Serial.println("and open http://192.168.4.1 in your browser");
  } else {
    Serial.printf("Connecting to %s\n", ssid);
    if (String(WiFi.SSID()) != String(ssid)) {
      WiFi.begin(ssid, password);
    }

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.print("Connected! Open http://");
    Serial.print(WiFi.localIP());
    Serial.println(" in your browser");
  }

  server.on("/all", HTTP_GET, []() {
    #ifdef NETDEBUG
    Serial.println("all");
    #endif
    sendAll();
    hasNotification = false;
  });

  server.on("/power", HTTP_GET, []() {    
    sendPower();
  });

  server.on("/power", HTTP_POST, []() {
    #ifdef NETDEBUG
    Serial.println("POWER: " + server.arg("value"));
    #endif
    String value = server.arg("value");
    setPower(value.toInt());    
    sendPower();
  });

  server.on("/solidColor", HTTP_GET, []() {
    sendSolidColor();
  });

  

  server.on("/solidColor", HTTP_POST, []() {
    #ifdef NETDEBUG
    Serial.println("RGB: " + server.arg("r") + "," + server.arg("g") + "," + server.arg("b"));
    #endif
    enableLoop = false;
    String r = server.arg("r");
    String g = server.arg("g");
    String b = server.arg("b");
    poiColor2 = poiColor1;
    poiColor1 = strip.Color(r.toInt(), g.toInt(), b.toInt());
    setSolidColor(r.toInt(), g.toInt(), b.toInt());
    sendSolidColor();
  });

  server.on("/compass", HTTP_POST, []() {
    compassDir = server.arg("value").toInt();
    Serial.print("Compass:"); Serial.println(compassDir);
    mode = COMPASS;
  
    String json = String(compassDir);
    server.send(200, "text/json", json);
    json = String();
  });

  server.on("/latlong", HTTP_GET, []() {
    sendLatLong();
  });
  
  server.on("/latlong", HTTP_POST, []() {
    char latitudeStr[64];
    server.arg("lat").toCharArray(latitudeStr,64);
    char longitudeStr[64];
    server.arg("long").toCharArray(longitudeStr, 64);
    
    latitude = strtod(latitudeStr, NULL);
    longitude = strtod(longitudeStr, NULL);

    dtostrf(latitude, 6, 15, latitudeStr);
    dtostrf(longitude, 6, 15, longitudeStr);
    
    saveLatLong();
    sendLatLong();
  });

  server.on("/vumeter", HTTP_POST, []() {
    vuPercent = server.arg("value").toInt();
    Serial.println("Received VU value : " + String(vuPercent));
    String json = String("{vuValue: " + String(vuPercent) + "}");
    server.send(200, "text/json", json);
    json = String();
    mode = VU_METER;    
  });

  server.on("/currposition", HTTP_POST, []() {
    Serial.println("currPos");
    char latitudeStr[64];
    server.arg("lat").toCharArray(latitudeStr,64);
    char longitudeStr[64];
    server.arg("long").toCharArray(longitudeStr, 64);

    // Store relative lat/long and heading
    currLat = strtod(latitudeStr, NULL);
    currLong = strtod(longitudeStr, NULL);
    compassDir = server.arg("heading").toInt();

    //Debug print difference vectors
    dtostrf(currLat - latitude, 6, 15, latitudeStr);
    dtostrf(currLong - longitude, 6, 15, longitudeStr);    
    Serial.print("Lat/Long: "); Serial.print(latitudeStr); Serial.print("/"); Serial.println(longitudeStr);

    // Return heading
    String json = String(compassDir);
    server.send(200, "text/json", json);
    json = String();
  });

  server.on("/pattern", HTTP_GET, []() {
    sendPattern();
  });

  server.on("/pattern", HTTP_POST, []() {
    enableLoop = false;
    hasNotification = false;
    String value = server.arg("value");
    setPattern(value.toInt());
    sendPattern();
  });

  server.on("/patternUp", HTTP_POST, []() {
    adjustPattern(true);
    sendPattern();
  });

  server.on("/patternDown", HTTP_POST, []() {
    adjustPattern(false);
    sendPattern();
  });

  server.on("/brightness", HTTP_GET, []() {
    sendBrightness();
  });

  server.on("/brightness", HTTP_POST, []() {
    String value = server.arg("value");
    setBrightness(value.toInt());
    sendBrightness();
  });

  server.on("/message", HTTP_POST, []() {
    String value = server.arg("value");
    // TODO: can this overflow?  Maybe use a queue
    message = message + ":::" + value;
    Serial.println("Message now: " + message);
    sendMessage();
  });

  server.on("/messageBroadcast", HTTP_POST, []() {
    String value = server.arg("value");
    // TODO: can this overflow?  Maybe use a queue
    message = value;
    hasNotification = true;
    resetNotify();
  });

  server.on("/size", HTTP_POST, []() {
    String value = server.arg("value");
    NUM_LEDS = value.toInt();
    #ifndef FAST_NEOPIXEL
    strip.updateLength(NUM_LEDS);
    #endif
    fastLedSetup();

    String json = String("{\"size\":") + String(NUM_LEDS) + String("}");
    server.send(200, "text/json", json);
    json = String();

    saveSize();
  });

  server.on("/bpm", HTTP_POST, []() {
    String value = server.arg("value");

    gBpm = value.toInt();
    fastLedSetup();

    String json = String("{\"bpm\":") + String(gBpm) + String("}");
    server.send(200, "text/json", json);
    json = String();
  });
  server.on("/brightnessUp", HTTP_POST, []() {
    adjustBrightness(true);
    sendBrightness();
  });

  server.on("/brightnessDown", HTTP_POST, []() {
    adjustBrightness(false);
    sendBrightness();
  });
    
  server.serveStatic("/index.htm", SPIFFS, "/index.htm");
  server.serveStatic("/index.html", SPIFFS, "/index.htm");
  server.serveStatic("/", SPIFFS, "/index.htm");

  server.serveStatic("/fonts", SPIFFS, "/fonts", "max-age=86400");
  server.serveStatic("/js", SPIFFS, "/js");
  server.serveStatic("/css", SPIFFS, "/css", "max-age=86400");
  server.serveStatic("/images", SPIFFS, "/images", "max-age=86400");  

  server.begin();

  Serial.println("HTTP server started");

  autoPlayTimeout = millis() + (autoPlayDurationSeconds * 1000);
}


void serverLoop(void) {
  server.handleClient();
  if (power == 0) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    FastLED.delay(15);
    return;
  }
}
#endif
