#include "nuba_wm.h"
#include "nuba_HTML.h"

#ifdef ESP8266
#include <schedule.h>
#endif

#define PIN_STAMP 0xABCD1234
#define EEPROM_SIZE 16
#define EEPROM_PIN_ADDR 0

namespace nuba {
  
  Manager::Manager() : _server(nullptr), _dns(nullptr), _debug(false), 
  _connectCallback(nullptr), _portalCallback(nullptr), _disconnectCallback(nullptr),
  _pinAuthenticated(false), _autoReconnect(true), _lastReconnectAttempt(0),
  _reconnectAttempts(0), _reconnectTimeout(60000), _disconnectTime(0),
  _isConfigPortalRunning(false), _backgroundRunning(false)
  #ifdef ESP32
  , _taskHandle(nullptr)
  #endif
  {
    _config = Config();
    _pinData.magic = 0;
    _pinData.isDefault = true;
    strcpy(_pinData.pin, "0000");
    
    initPINStorage();
    loadPIN();
  }
  // END OF Manager() SCOPE
  
  Manager::~Manager() {
    if (_server) delete _server;
    if (_dns) delete _dns;
    
    #ifdef ESP32
    _preferences.end();
    if (_taskHandle) vTaskDelete(_taskHandle);
    #else
    if (_backgroundRunning) {
      _ticker.detach();
    }
    #endif
  }
  
  void Manager::initPINStorage() {
    #ifdef ESP32
    _preferences.begin("nuba", false);
    #else
    EEPROM.begin(EEPROM_SIZE);
    #endif
  }
  // END OF initPINStorage() SCOPE
  
  bool Manager::loadPIN() {
    #ifdef ESP32
    uint32_t magic = _preferences.getUInt("magic", 0);
    if (magic == PIN_STAMP) {
      String pin = _preferences.getString("pin", "0000");
      bool isDefault = _preferences.getBool("isDefault", true);
      
      strncpy(_pinData.pin, pin.c_str(), 4);
      _pinData.pin[4] = '\0';
      _pinData.isDefault = isDefault;
      _pinData.magic = magic;
      return true;
    }
    #else
    uint32_t magic;
    EEPROM.get(EEPROM_PIN_ADDR, magic);
    
    if (magic == PIN_STAMP) {
      EEPROM.get(EEPROM_PIN_ADDR + 4, _pinData.pin);
      EEPROM.get(EEPROM_PIN_ADDR + 9, _pinData.isDefault);
      _pinData.magic = magic;
      return true;
    }
    #endif
    
    strcpy(_pinData.pin, "0000");
    _pinData.isDefault = true;
    _pinData.magic = PIN_STAMP;
    return false;
  }
  // END OF loadPIN() SCOPE
  
  bool Manager::savePIN(const char* pin, bool isDefault) {
    if (strlen(pin) != 4) {
      log("PIN must be 4 digits");
      return false;
    }
    
    strncpy(_pinData.pin, pin, 4);
    _pinData.pin[4] = '\0';
    _pinData.isDefault = isDefault;
    _pinData.magic = PIN_STAMP;
    
    #ifdef ESP32
    _preferences.putUInt("magic", PIN_STAMP);
    _preferences.putString("pin", String(pin));
    _preferences.putBool("isDefault", isDefault);
    #else
    EEPROM.put(EEPROM_PIN_ADDR, PIN_STAMP);
    EEPROM.put(EEPROM_PIN_ADDR + 4, _pinData.pin);
    EEPROM.put(EEPROM_PIN_ADDR + 9, isDefault);
    EEPROM.commit();
    #endif
    
    return true;
  }
  // END OF savePIN() SCOPE
  
  bool Manager::verifyPIN(const char* pin) {
    return strcmp(_pinData.pin, pin) == 0;
  }
  // END OF verifyPIN() SCOPE
  
  void Manager::resetPIN() {
    #ifdef ESP32
    _preferences.clear();
    #else
    for (int i = 0; i < EEPROM_SIZE; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
    #endif
    
    strcpy(_pinData.pin, "0000");
    _pinData.isDefault = true;
    _pinData.magic = 0;
    
    log("PIN reset to default");
  }
  // END OF resetPin() SCOPE
  
  bool Manager::init(const char* apSSID, const char* apPassword) {
    log("Initialize nuba_wm");
    
    if (apSSID) strncpy(_config.ssid, apSSID, 31);
    if (apPassword) strncpy(_config.password, apPassword, 63);
    
    WiFi.persistent(true);
    
    WiFi.mode(WIFI_STA);
    delay(100);
    
    String storedSSID = WiFi.SSID();
    if (storedSSID.length() > 0) {
      log("SSID found: " + storedSSID);
      log("Connecting...");
      WiFi.begin();
      
      uint32_t start = millis();
      while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start > _reconnectTimeout) { 
          log("Connection has timed out after " + String(_reconnectTimeout/1000) + " seconds");
          log("Running SoftAP. Portal opened and ready to configure.");
          
          openPortal(apSSID, apPassword);
          startBackground();
          return false;
        }
        yield();
        delay(100);
      }
      
      log("Connected. IP: " + WiFi.localIP().toString());
      if (_connectCallback) _connectCallback();
      startBackground();
      return true;
      
    } else {
      log("Stored credentials not found");
      log("Running SoftAP. Portal opened and ready to configure");
      openPortal(apSSID, apPassword);
      
      while (true) {
        run();
        if (WiFi.status() == WL_CONNECTED) {
          log("Connected. IP: " + WiFi.localIP().toString());
          if (_connectCallback) _connectCallback();
          startBackground();
          return true;
        }
        yield();
        delay(10);
      }
    }
  }

  void Manager::startBackground() {
    if (_backgroundRunning) return;
    
    #ifdef ESP32
    xTaskCreatePinnedToCore(_taskWorker, "nuba_wm", 4096, this, 1, &_taskHandle, 1);
    #else
    _ticker.attach_ms(50, _tickWorker, this);
    #endif
    
    _backgroundRunning = true;
    log("Background worker started");
  }

  #ifdef ESP32
  void Manager::_taskWorker(void* param) {
    Manager* manager = (Manager*)param;
    for (;;) {
      manager->run();
      delay(10);
    }
  }
  #else
  void Manager::_tickWorker(Manager* manager) {
    schedule_function([manager]() { manager->run(); });
  }
  #endif
  // END OF init() SCOPE
  
  void Manager::openPortal(const char* apSSID, const char* apPassword) {
    if (apSSID) strncpy(_config.ssid, apSSID, 31);
    if (apPassword) strncpy(_config.password, apPassword, 63);
    
    log("Opening portal...");
    log("Free Heap: " + String(ESP.getFreeHeap()));
    
    WiFi.mode(WIFI_AP_STA);
    delay(200);
    
    WiFi.softAPConfig(_config.apIP, _config.gateway, _config.subnet);
    delay(100);
    
    bool apStarted = WiFi.softAP(_config.ssid, _config.password, _config.channel, _config.hidden);
    delay(500);
    
    if (!apStarted) {
      log("SoftAP failed to run. Retrying...");
      delay(1000);
      WiFi.softAP(_config.ssid, _config.password, _config.channel, _config.hidden);
      delay(500);
    }
    
    log("AP SSID: " + String(_config.ssid));
    log("AP IP: " + WiFi.softAPIP().toString());
    
    if (!_dns) _dns = new DNSServer();
    _dns->setErrorReplyCode(DNSReplyCode::NoError);
    _dns->start(53, "*", _config.apIP);
    delay(100);
    
    log("DNS server started");
    
    if (!_server) _server = new WebServerClass(80);
    setupServer();
    _server->begin();
    delay(100);
    
    log("Web server started");
    log("Key: " + String(_pinData.isDefault ? "Default" : "User preferences"));
    
    _startTime = millis();
    _pinAuthenticated = false;
    _isConfigPortalRunning = true;
    
    if (_portalCallback) _portalCallback();
  }
  // END OF openPortal() SCOPE
  
  void Manager::setupServer() {
    _server->on("/", [this]() { handleRoot(); });
    _server->on("/scan", [this]() { handleScan(); });
    _server->on("/save", [this]() { handleSave(); });
    _server->on("/info", [this]() { handleInfo(); });
    _server->on("/reset", [this]() { handleReset(); });
    _server->on("/verify-pin", [this]() { handleVerifyPIN(); });
    _server->on("/update-pin", [this]() { handleUpdatePIN(); });
    _server->on("/check-pin", [this]() { handleCheckPIN(); });
    _server->onNotFound([this]() { handleRoot(); });
  }
  // END OF setupServer() SCOPE
  
  void Manager::handleRoot() {
    // log("Serving web page");
    _server->send_P(200, PSTR("text/html"), NUBA_PORTAL_HTML);
  }
  // END OF handleRoot() SCOPE
  
  void Manager::handleCheckPIN() {
    String json = "{";
    json += "\"authenticated\":" + String(_pinAuthenticated ? "true" : "false") + ",";
    json += "\"isDefault\":" + String(_pinData.isDefault ? "true" : "false");
    json += "}";
    _server->send(200, "application/json", json);
  }
  // END OF handleCheckPIN() SCOPE
  
  void Manager::handleVerifyPIN() {
    if (!_server->hasArg("pin")) {
      _server->send(400, "application/json", "{\"success\":false,\"message\":\"PIN required\"}");
      return;
    }
    
    String pin = _server->arg("pin");
    
    if (pin.length() != 4) {
      _server->send(400, "application/json", "{\"success\":false,\"message\":\"PIN must be 4 digits\"}");
      return;
    }
    
    if (verifyPIN(pin.c_str())) {
      _pinAuthenticated = true;
      log("PIN verified");
      
      String json = "{\"success\":true,\"isDefault\":" + String(_pinData.isDefault ? "true" : "false") + "}";
      _server->send(200, "application/json", json);
    } else {
      log("Invalid PIN attempt");
      _server->send(401, "application/json", "{\"success\":false,\"message\":\"Invalid PIN\"}");
    }
  }
  // END OF handleVerifyPIN() SCOPE
  
  void Manager::handleUpdatePIN() {
    if (!_pinAuthenticated) {
      _server->send(401, "application/json", "{\"success\":false,\"message\":\"Not authenticated\"}");
      return;
    }
    
    if (!_server->hasArg("newPin")) {
      _server->send(400, "application/json", "{\"success\":false,\"message\":\"New PIN required\"}");
      return;
    }
    
    String newPin = _server->arg("newPin");
    
    if (newPin.length() != 4) {
      _server->send(400, "application/json", "{\"success\":false,\"message\":\"PIN must be 4 digits\"}");
      return;
    }
    
    for (int i = 0; i < 4; i++) {
      if (!isdigit(newPin[i])) {
        _server->send(400, "application/json", "{\"success\":false,\"message\":\"PIN must contain only digits\"}");
        return;
      }
    }
    
    if (newPin == "0000") {
      _server->send(400, "application/json", "{\"success\":false,\"message\":\"Cannot use default PIN\"}");
      return;
    }
    
    if (savePIN(newPin.c_str(), false)) {
      log("PIN updated");
      _server->send(200, "application/json", "{\"success\":true,\"message\":\"PIN updated successfully\"}");
    } else {
      log("Failed to save PIN");
      _server->send(500, "application/json", "{\"success\":false,\"message\":\"Failed to save PIN\"}");
    }
  }
  // END OF handleUpdatePIN() SCOPE
  
  void Manager::handleScan() {
    if (!_pinAuthenticated) {
      _server->send(401, "application/json", "{\"error\":\"Not authenticated\"}");
      return;
    }
    
    log("Scanning for available networks around you...");
    
    WiFi.disconnect();
    int n = WiFi.scanNetworks(false, true);
    
    log("Found " + String(n) + " networks");
    
    String json = "{\"networks\":[";
    
    for (int i = 0; i < n && i < 15; i++) { 
      if (i > 0) json += ",";
      
      String ssid = WiFi.SSID(i);
      ssid.replace("\"", "\\\""); 
      
      json += "{\"ssid\":\"" + ssid + "\",";
      json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
      #ifdef ESP32
      json += "\"secure\":" + String(WiFi.encryptionType(i) != WIFI_AUTH_OPEN) + "}";
      #else
      json += "\"secure\":" + String(WiFi.encryptionType(i) != ENC_TYPE_NONE) + "}";
      #endif
      
      yield();
    }
    
    json += "]}";
    WiFi.scanDelete();
    
    _server->send(200, "application/json", json);
    log("Scan complete");
  }
  // END OF handleScan() SCOPE
  
  void Manager::handleSave() {
    if (!_pinAuthenticated) {
      _server->send(401, "application/json", "{\"success\":false,\"message\":\"Not authenticated\"}");
      return;
    }
    
    if (!_server->hasArg("ssid")) {
      _server->send(400, "application/json", "{\"success\":false}");
      return;
    }
    
    String ssid = _server->arg("ssid");
    String pass = _server->arg("pass");
    
    strncpy(_creds.ssid, ssid.c_str(), 31);
    _creds.ssid[31] = '\0';
    strncpy(_creds.password, pass.c_str(), 63);
    _creds.password[63] = '\0';
    
    _server->send(200, "application/json", "{\"success\":true}");
    
    delay(100);
    
    log("Connecting to: " + String(_creds.ssid));
    if (connectWiFi(_creds.ssid, _creds.password)) {
      log("Connected!");
      if (_connectCallback) _connectCallback();
      delay(100);
      closePortal();
    } else {
      log("Connection failed. Running SoftAP and preparing portal...");
      
      WiFi.disconnect(false);
      delay(200);
      
      WiFi.mode(WIFI_AP_STA);
      delay(200);
      
      WiFi.softAPConfig(_config.apIP, _config.gateway, _config.subnet);
      WiFi.softAP(_config.ssid, _config.password, _config.channel, _config.hidden);
      delay(500);
    }
  }
  // END OF handleSave() SCOPE
  
  void Manager::handleInfo() {
    if (!_pinAuthenticated) {
      _server->send(401, "application/json", "{\"error\":\"Not authenticated\"}");
      return;
    }
    
    String json = "{";
    #ifdef ESP32
    json += "\"chipId\":\"" + String((uint32_t)ESP.getEfuseMac()) + "\",";
    #else
    json += "\"chipId\":\"" + String(ESP.getChipId()) + "\",";
    #endif
    json += "\"freeHeap\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"ssid\":\"" + WiFi.SSID() + "\",";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\"";
    json += "}";
    _server->send(200, "application/json", json);
  }
  // END OF handleInfo() SCOPE
  
  void Manager::handleReset() {
    if (!_pinAuthenticated) {
      _server->send(401, "application/json", "{\"success\":false,\"message\":\"Not authenticated\"}");
      return;
    }
    
    _server->send(200, "application/json", "{\"success\":true}");
    delay(500);
    
    clear();
  }
  // END OF handleReset() SCOPE
  
  bool Manager::connectWiFi(const char* ssid, const char* password) {
    log("Connecting to WiFi...");
    
    WiFi.disconnect(false);
    delay(100);
    
    WiFi.mode(WIFI_STA);
    delay(100);
    
    WiFi.persistent(true);
    #ifdef ESP8266
    WiFi.setAutoConnect(true);
    #endif
    WiFi.setAutoReconnect(true);
    
    WiFi.begin(ssid, password);
    
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED) {
      if (millis() - start > 15000) {
        log("Connection has timed out");
        return false;
      }
      yield();
      delay(100);
    }
    
    log("Connected!");
    return true;
  }
  // END OF connectWiFi() SCOPE
  
  void Manager::closePortal() {
    if (_server) {
      _server->stop();
      delay(100);
      delete _server;
      _server = nullptr;
    }
    
    if (_dns) {
      _dns->stop();
      delay(100);
      delete _dns;
      _dns = nullptr;
    }
    
    WiFi.softAPdisconnect(true);
    delay(200);
    
    _isConfigPortalRunning = false;
    log("Portal closed");
  }
  // END OF closePortal() SCOPE
  
  void Manager::run() {
    if (_dns) {
      _dns->processNextRequest();
    }
    
    if (_server) {
      _server->handleClient();
    }
    
    if (_autoReconnect) {
      handleAutoReconnect();
    }
    
    yield();
    
    if (_isConfigPortalRunning && WiFi.SSID().length() == 0) {
      if (_config.timeout > 0 && (millis() - _startTime > _config.timeout)) {
        log("Portal has timed out");
        _startTime = millis();
      }
    }
  }
  // END OF run() SCOPE
  
  void Manager::clear() {
    log("Clearing WiFi Settings...");
    WiFi.disconnect(true); 
    delay(500);
    ESP.restart();
  }
  // END OF clear() SCOPE
  
  void Manager::portalTimeout(uint32_t seconds) {
    _config.timeout = seconds * 1000;
  }
  
  void Manager::setIP(IPAddress ip) {
    _config.apIP = ip;
    _config.gateway = ip;
    _config.subnet = IPAddress(255, 255, 255, 0);
  }
  
  void Manager::setIP(uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4) {
    setIP(IPAddress(o1, o2, o3, o4));
  }
  
  void Manager::debug(bool enable) {
    _debug = enable;
  }
  
  void Manager::autoReconnect(bool enable) {
    _autoReconnect = enable;
    log("Auto reconnect " + String(enable ? "enabled" : "disabled"));
  }
  
  void Manager::reconnectTimeout(uint32_t seconds) {
    _reconnectTimeout = seconds * 1000;
    log("Reconnect timeout set to " + String(seconds) + " seconds");
  }
  
  void Manager::onConnect(std::function<void()> cb) {
    _connectCallback = cb;
  }

  void Manager::onConfigPortalStart(std::function<void()> cb) {
    _portalCallback = cb;
  }

  void Manager::onDisconnect(std::function<void()> cb) {
    _disconnectCallback = cb;
  }

  void Manager::handleAutoReconnect() {
    if (_isConfigPortalRunning) {
      return;
    }
    
    static bool wasConnected = false;
    bool isConnected = (WiFi.status() == WL_CONNECTED);
    
    if (wasConnected && !isConnected) {
      log("WiFi disconnected. Reconnecting...");
      _reconnectAttempts = 0;
      _disconnectTime = millis();
      _lastReconnectAttempt = 0;
      
      if (_disconnectCallback) {
        _disconnectCallback();
      }
    }
    
    wasConnected = isConnected;
    
    if (!isConnected && WiFi.SSID().length() > 0) {
      uint32_t now = millis();
      
      if (now - _disconnectTime >= _reconnectTimeout) {
        log("Reconnect timeout reached (" + String(_reconnectTimeout/1000) + "s). Running SoftAP and preparing portal...");
        
        _disconnectTime = now; 
        _reconnectAttempts = 0;
        
        openPortal(_config.ssid, _config.password);
        return;
      }
      
      uint32_t interval;
      if (_reconnectTimeout <= 30000) {          
        interval = 3000;
      } else if (_reconnectTimeout <= 60000) {   
        interval = 5000;
      } else if (_reconnectTimeout <= 120000) {  
        interval = 7000;
      } else {                                    
        interval = 10000;
      }
      
      if (now - _lastReconnectAttempt >= interval) {
        _lastReconnectAttempt = now;
        _reconnectAttempts++;
        
        uint32_t timeElapsed = (now - _disconnectTime) / 1000;
        uint32_t timeRemaining = (_reconnectTimeout - (now - _disconnectTime)) / 1000;
        
        log("Attempt: " + String(_reconnectAttempts) + 
            " (" + String(timeElapsed) + "s elapsed, " + 
            String(timeRemaining) + "s remaining)");
        
        String savedSSID = WiFi.SSID();
        String savedPass = WiFi.psk();
        
        log("Reconnect to: " + savedSSID);
        
        WiFi.disconnect(false);
        delay(100);
        
        WiFi.mode(WIFI_STA);
        delay(100);
        
        WiFi.begin(savedSSID.c_str(), savedPass.c_str());
        
        uint32_t start = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
          yield();
          delay(100);
        }
        
        if (WiFi.status() == WL_CONNECTED) {
          log("Reconnected. IP: " + WiFi.localIP().toString());
          _reconnectAttempts = 0;
          _disconnectTime = 0;
          
          if (_connectCallback) {
            _connectCallback();
          }
        } else {
          log("Attempt: " + String(_reconnectAttempts) + " failed");
        }
      }
    }
  }
  // END OF handleAutoReconnect() SCOPE
  
  void Manager::netInfo() {
    if (!_debug) return;
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Status    : Connected");
      Serial.println("Mode      : STA");
      Serial.print("SSID      : ");
      Serial.println(WiFi.SSID());
      Serial.print("Channel   : ");
      Serial.println(WiFi.channel());
      Serial.print("RSSI      : ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
      Serial.print("MAC Addr. : "); 
      Serial.println(WiFi.macAddress());
      Serial.print("IP Addr.  : ");
      Serial.println(WiFi.localIP());
      Serial.print("Gateway   : ");
      Serial.println(WiFi.gatewayIP());
      Serial.print("Netmask   : ");
      Serial.println(WiFi.subnetMask());
      Serial.print("DNS       : ");
      Serial.println(WiFi.dnsIP());
    } else {
      Serial.println("Status    : Disconnected");
      if (_isConfigPortalRunning) {
        Serial.println("Mode      : AP");
        Serial.print("SSID      : ");
        Serial.println(_config.ssid);
        Serial.print("IP        : ");
        Serial.println(WiFi.softAPIP());
        
        String savedSSID = WiFi.SSID();
        if (savedSSID.length() > 0) {
          Serial.print("Stored    : ");
          Serial.println(savedSSID);
        }
      }
    }
    
    Serial.print("Reconnect : ");
    Serial.println(_autoReconnect ? "true" : "false");
    Serial.print("Free Heap : ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
  }
  // END OF printDebugInfo() SCOPE
  
  void Manager::log(const String& msg) {
    if (_debug) {
      Serial.println("[nuba] " + msg);
    }
  }
  
}