
#ifndef WIFI_H
#define WIFI_H
/*
  mesh.h
  Wifi messaging that communicates to other APs in the "mesh".
  All code based on ESP8266wifimesh - https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFiMesh
  Their license and description follows:

  ESP8266WiFiMesh.h - Mesh network node
  Sets up a Mesh Node which acts as a router, creating a Mesh Network with other nodes. All information
  is passed in both directions, but it is up to the user what the data sent is and how it is dealt with.
 
  Copyright (c) 2015 Julian Fell. All rights reserved.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

String manageRequest(String request);
#define SERVER_NAME "192.168.4.1"

/**
 * Callback for when other nodes send you data
 *
 * @request The string received from another node in the mesh
 * @returns The string to send back to the other node
 */
String manageRequest(String request)
{
  count = 0;
  /* Print out received message */
  Serial.print("received: ");
  Serial.println(request);

  if (request.substring(0, 7) == "MESSAGE") {
    message = request.substring(8);
  }

  if (request.indexOf("HTTP/1.1 200 OK") > -1) {
    peerCount++;
    drawPeers();
    delay(250);
    return "";
  } 
  
  if (request.substring(0, 4) == "MODE"){    
    int newMode = request.substring(5).toInt();
    
    // Currently does sparkle, TODO: add ping/flair per color animation
    if (newMode == 2){
      strip.clear();
      
      // So annoying
      Serial.println("tweaking for " + mode);
      delay(0);
      modeTweak(mode);
      
      char response[60];
      sprintf(response, "ACK ping mode: %d <3 %d.", mode, ESP.getChipId());  
    
      return response;
    }
  }
}

bool waitForClientManual(WiFiClient curr_client, int max_wait)
{
  int wait = max_wait;
  while(curr_client.connected() && !curr_client.available() && wait--)
    delay(3);

  if (wait <= 0){
    Serial.println("Wait timeout.");
  }

  /* Return false if the client isn't ready to communicate */
  if (WiFi.status() == WL_DISCONNECTED || !curr_client.connected()) {
    Serial.println("Disconnected");
    return false;
  }
  
  return true;
}

bool exchangeInfoManual(String message, WiFiClient curr_client)
{ 
  curr_client.println( message.c_str() );

  if (!waitForClientManual(curr_client, 5000))
    return false;

  String response = curr_client.readStringUntil('\r');
  curr_client.readStringUntil('\n');

  if (response.length() <= 2) {
    Serial.println("invalid response length, return");
    return false;
  }

  /* Pass data to user callback */
  manageRequest(response);
  return true;
}

void connectToNodeManual(String target_ssid, String message)
{
  WiFiClient curr_client;
  WiFi.begin( target_ssid.c_str() );

  int wait = 5000;
  while((WiFi.status() == WL_DISCONNECTED) && wait--)
    delay(3);

  /* If the connection timed out */
  if (WiFi.status() != 3) {
    Serial.println("timeout, return");
    return;
  }

  /* Connect to the node's server */
  // TODO: make server ip constant
  if (!curr_client.connect(SERVER_NAME, 80)) {
    Serial.println("no connection, return");
    return;
  }

  if (!exchangeInfoManual(message, curr_client)) {
    Serial.println("Failed exchange, return");
    return;
  }

  curr_client.stop();
  WiFi.disconnect();
}
String _ssid_prefix = "Rainbow Snake";
void sendMessageManual(String message)
{
  /* Scan for APs */
  int n = WiFi.scanNetworks();
  delay(0);

  for (int i = 0; i < n; ++i) {
    String current_ssid = WiFi.SSID(i);
    int index = current_ssid.indexOf( _ssid_prefix);
    uint32_t target_chip_id = (current_ssid.substring(index + _ssid_prefix.length())).toInt();

    /* Connect to any _suitable_ APs which contain _ssid_prefix */
    if (index >= 0) {
      Serial.println("Updating: " + current_ssid);
      WiFi.mode(WIFI_STA);
      delay(0);
      connectToNodeManual(current_ssid, message);
      WiFi.mode(WIFI_AP_STA);
      delay(0);
    }
  }
}

void transmitAnimation(CRGB color) {
  strip.clear();
  strip.show();
  peerCount = 0;

  for (int i=0; i < 25; i++){
    bubble(color, true);
  }
  for (int i=0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}

String URLEncode(const char* msg)
{
    const char *hex = "0123456789abcdef";
    String encodedMsg = "";

    while (*msg!='\0'){
        if( ('a' <= *msg && *msg <= 'z')
                || ('A' <= *msg && *msg <= 'Z')
                || ('0' <= *msg && *msg <= '9') ) {
            encodedMsg += *msg;
        } else {
            encodedMsg += '%';
            encodedMsg += hex[*msg >> 4];
            encodedMsg += hex[*msg & 15];
        }
        msg++;
    }
    return encodedMsg;
}


/**
 * updateMeshMode
 * 
 * Updates all scanned APs with the current message.
 */
void updateMeshMessage(){
  transmitAnimation(CRGB::Blue);

  strip.clear();
  char buf[1024];
  message.toCharArray(buf, 2048);
  String urlMessage = URLEncode(buf);
    
  String request = String("POST /messageBroadcast?value=") + String(urlMessage) + String(" HTTP/1.1\r\n") +
               String("Host: ") + String(SERVER_NAME) + String("\r\n") + 
               String("Connection: close\r\n\r\n");
  Serial.println(request);
  sendMessageManual(request);
  count = 0;
}


/**
 * updateMeshMode
 * 
 * Updates all scanned APs with the current mode.
 */
void updateMeshMode() {
  transmitAnimation(CRGB::White);
  FastLED.show();
  
  String request = String("POST /pattern?value=") + String(mode) + String(" HTTP/1.1\r\n") +
               String("Host: ") + String(SERVER_NAME) + String("\r\n") + 
               String("Connection: close\r\n\r\n");
               
  Serial.println(request);
  sendMessageManual(request);
  count = 0;
}

/**
 * Counts mesh nodes and renders to display
 */
void countMeshNodes() {
    /* Scan for APs */
  int n = WiFi.scanNetworks();
  delay(0);

  int foundAPs = 0;

  for (int i = 0; i < n; ++i) {
    String current_ssid = WiFi.SSID(i);
    int index = current_ssid.indexOf( _ssid_prefix);
    uint32_t target_chip_id = (current_ssid.substring(index + _ssid_prefix.length())).toInt();

    /* Connect to any _suitable_ APs which contain _ssid_prefix */
    if (index >= 0) {
      strip.setPixelColor(foundAPs, 255, 0, 0);
      strip.show();    
      delay(50);
      foundAPs++;
    }
  }
  strip.show();
  Serial.println("Found " + String(foundAPs) + " AP[s]");

  while (buttonOn()) {
    delay(25);
  }
}

void startCountAnimation(){
  // Position white pixels
  for (int i=0; i < 5; i++){
    strip.setPixelColor(i, 0xFFFFFF); 
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

void buttonCountMeshNodes() {
  if (buttonOn()) {
    startCountAnimation();
    countMeshNodes();
  } else {
    strip.clear();
    strip.show();
  }
}
#endif
