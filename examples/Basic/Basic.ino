/**
 * nuba_wm - Simple and more secure WiFi Manager for ESP32/ESP8266
 * This basic code example runs the WiFi Manager using the default configuration. 
 * Read the documentation at https://github.com/readonlymem0ry/nuba_wm
 * 
 * @version 1.0.0
 * @author readonlymem0ry
 * @license MIT
 */

#include "nuba_wm.h"

nuba::Manager wm;

void setup() {
  Serial.begin(115200);
  wm.setDebug(true);
  wm.begin();
}

void loop() {
  wm.run();
}
