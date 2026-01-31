/**
 * nuba_wm - Simple and more secure WiFi Manager for ESP32/ESP8266
 * Read the documentation at https://github.com/readonlymem0ry/nuba_wm
 * 
 * @version 1.0
 * @author readonlymem0ry
 * @license MIT
 */

#ifndef NUBA_H
#define NUBA_H

#include <Arduino.h>
#include <functional>

#ifdef ESP32
  #include <WiFi.h>
  #include <WebServer.h>
  #include <DNSServer.h>
  #include <Preferences.h>
  using WebServerClass = WebServer;
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <DNSServer.h>
  #include <EEPROM.h>
  #include <Ticker.h>
  using WebServerClass = ESP8266WebServer;
#endif

namespace nuba {

struct Config {
  char ssid[32] = "nuba AP";
  char password[64] = "";
  uint8_t channel = 1;
  bool hidden = false;
  uint32_t timeout = 180000; 
  IPAddress apIP = IPAddress(192, 168, 4, 1);
  IPAddress gateway = IPAddress(192, 168, 4, 1);
  IPAddress subnet = IPAddress(255, 255, 255, 0);
};

struct Credentials {
  char ssid[32];
  char password[64];
};

struct PINData {
  char pin[5];  
  bool isDefault;
  uint32_t magic; 
};

class Manager {
public:
  Manager();
  ~Manager();
  
  bool init(const char* apSSID = nullptr, const char* apPassword = nullptr);
  void openPortal(const char* apSSID = nullptr, const char* apPassword = nullptr);
  void run();
  void clear();
  
  void setConfig(const Config& cfg);
  void portalTimeout(uint32_t seconds);
  void setIP(IPAddress ip);
  void setIP(uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4);
  void debug(bool enable);
  void autoReconnect(bool enable);
  void reconnectTimeout(uint32_t seconds);
  void netInfo();
  void onConnect(std::function<void()> cb);
  void onConfigPortalStart(std::function<void()> cb);
  void onDisconnect(std::function<void()> cb);

private:
  WebServerClass* _server;
  DNSServer* _dns;
  Config _config;
  Credentials _creds;
  PINData _pinData;
  bool _debug;
  uint32_t _startTime;
  bool _pinAuthenticated;
  bool _autoReconnect;
  uint32_t _lastReconnectAttempt;
  uint8_t _reconnectAttempts;
  uint32_t _reconnectTimeout;      
  uint32_t _disconnectTime;        
  bool _isConfigPortalRunning;    
  std::function<void()> _connectCallback;
  std::function<void()> _portalCallback;
  std::function<void()> _disconnectCallback;
  bool _backgroundRunning;
  
  #ifdef ESP32
  Preferences _preferences;
  TaskHandle_t _taskHandle;
  static void _taskWorker(void* param);
  #else
  Ticker _ticker;
  static void _tickWorker(Manager* manager);
  #endif
  
  void setupServer();
  void handleRoot();
  void handleScan();
  void handleSave();
  void handleInfo();
  void handleReset();
  void handleVerifyPIN();
  void handleUpdatePIN();
  void handleCheckPIN();
  String getPage();
  String scanNetworks();
  bool connectWiFi(const char* ssid, const char* password);
  void closePortal();
  void log(const String& msg);
  void handleAutoReconnect();
  
  void initPINStorage();
  bool loadPIN();
  bool savePIN(const char* pin, bool isDefault);
  bool verifyPIN(const char* pin);
  void resetPIN();
  void startBackground();
};

} 

#endif
