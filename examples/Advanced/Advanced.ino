/**
 * nuba_wm - Simple and more secure WiFi Manager for ESP32/ESP8266
 * Read the documentation at https://github.com/readonlymem0ry/nuba_wm
 * 
 * @version 1.0
 * @author readonlymem0ry
 * @license MIT
 */

#include "nuba_wm.h"

nuba::Manager wm;

unsigned long lastInfoTime = 0;

void setup() {
  Serial.begin(115200);
  
  wm.debug(true);             // Enable debug mode and display it on Serial Monitor
  wm.setIP(192, 168, 10, 1);  // Set custom IP Address for AP mode

  wm.portalTimeout(180);      // Set the portal timeout to 3 minutes
  wm.autoReconnect(true);     // Enable auto-reconnect features
  
  // Set the reconnect timeout, It will return to AP mode 
  // if your ESP can't reconnect to saved WiFi profile after 1 minutes
  wm.reconnectTimeout(60);   
  
  wm.init("nuba AP", "ilovecoffee");  // Initialize with custom SSID and password
}

void loop() {
  // Displays network info on Serial Monitor every 5 seconds
  if (millis() - lastInfoTime > 5000) {
    lastInfoTime = millis();
    wm.netInfo();
  }
}