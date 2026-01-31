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

void setup() {
  Serial.begin(115200);
  wm.debug(true);
  wm.init();
}

void loop() {
  
}
