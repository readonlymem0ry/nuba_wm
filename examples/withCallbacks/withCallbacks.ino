/**
 * nuba_wm - Simple and more secure WiFi Manager for ESP32/ESP8266
 * This complete code example runs the WiFi Manager using custom configuration and callbacks based on user preferences. 
 * Read the documentation at https://github.com/readonlymem0ry/nuba_wm
 * 
 * @version 1.0.0
 * @author readonlymem0ry
 * @license MIT
 */

#include "nuba_wm.h"

nuba::Manager wm;

#define RESET_BUTTON 0    // D3 on NodeMCU (ESP8266) OR GPIO0 on ESP32
#define STATUS_LED 2      // D4 on NodeMCU (ESP8266) or GPIO2 on ESP32

unsigned long lastButtonPress = 0;
unsigned long lastStatusCheck = 0;

// Callback to run some task if WiFi Connected
void onWiFiConnected() {
  Serial.println("\nWiFi Connected!");
  digitalWrite(STATUS_LED, LOW);
  wm.printDebugInfo();
}

// Callback to run some task if WiFi Disconnected
void onWiFiDisconnected() {
  Serial.println("\nWiFi Disconnected!");
  Serial.println("Reconnecting...");
  digitalWrite(STATUS_LED, HIGH);
}

// Callback to display debug messages when Configuration Portal started
void onConfigPortalStarted() {
  Serial.println("\nConfig Portal Started");
  Serial.println("Connect to AP: nuba");
  Serial.println("Password: ilovecoffee");
  Serial.println("URL: http://192.168.10.1");   // Use custom IP Address for SoftAP
  Serial.println("PIN: Default");
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nnuba_wm Complete Example\n");
  
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, HIGH);
  
  wm.setDebug(true);    // Enable debug message display on Serial Monitor
  wm.setTimeout(300);   // Set timeout for configuration portal in seconds. The portal will be available for 300 seconds.
  wm.setIP(
    IPAddress(192, 168, 10, 1),   // IP Address
    IPAddress(192, 168, 10, 1),   // Gateway Address
    IPAddress(255, 255, 255, 0)   // Netmask
  );
  wm.autoReconnect(true);   // Enable auto-reconnect
  
  // Running some callbacks 
  wm.onConnect(onWiFiConnected);
  wm.onDisconnect(onWiFiDisconnected);
  wm.onConfigPortalStart(onConfigPortalStarted);
  
  wm.begin("nuba AP", "ilovecoffee");
  
}

void loop() {
  wm.run();   // Keep running
  
  // Push button action reading
  if (digitalRead(RESET_BUTTON) == LOW) {
    if (millis() - lastButtonPress > 50) {
      lastButtonPress = millis();
      Serial.println("\nReset button pressed. Keep hold for 3 seconds...");
      delay(3000);
      
      if (digitalRead(RESET_BUTTON) == LOW) {
        Serial.println("Resetting WiFi credentials...\n");
        wm.reset();
      } else {
        Serial.println("Reset cancelled\n");
      }
    }
  }
  
  // System status checking every 10 seconds
  if (millis() - lastStatusCheck >= 10000) {
    lastStatusCheck = millis();
    
    Serial.print("Uptime: ");
    Serial.print(millis() / 1000);
    Serial.print("s | WiFi: ");
    
    if (wm.isConnected()) {
      Serial.print("Connected. SSID: ");
      Serial.print(wm.getSSID());
      Serial.print(" | IP: ");
      Serial.println(wm.getIP());
    } else {
      Serial.println("Disconnected");
    }
  }
  
  if (wm.isConnected()) {
    // Write your other task code here
  }
  
  delay(10);
}