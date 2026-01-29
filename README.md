<h2>nuba_wm Function Reference</h2>
<p><em>A more secure and simple WiFi manager library for ESP8266/ESP32</em></p>

<hr>

<h3>Core Functions</h3>

<h4><code>begin()</code></h4>
<p>Initialize the WiFi Manager with the SSID and password for the Access Point (softAP). If a Wi-Fi connection is saved, it will automatically connect. If it fails or is not configured, the configuration portal will open.</p>
<pre>wm.begin();
wm.begin("nuba");
wm.begin("nuba", "ilovecoffee");</pre>

<h4><code>init()</code></h4>
<p>Alias ​​of <code>begin()</code>. Same function, you can use either one depending on your preference.</p>
<pre>wm.init();
wm.init("nuba", "ilovecoffee");</pre>

<h4><code>run()</code></h4>
<p>Handle the web server, DNS server, and auto-reconnect. <strong>Must be called inside the loop()</strong>function to keep WiFi Manager running properly.</p>
<pre>void loop() {
  wm.run();
}</pre>

<h4><code>startConfigPortal()</code></h4>
<p>Force open the config portal in AP mode. Useful for manually reconfiguring WiFi, for example, using physical buttons.</p>
<pre>wm.startConfigPortal();
wm.startConfigPortal("nuba AP");</pre>

<h4><code>reset()</code></h4>
<p>Clear the stored WiFi credentials and restarts the ESP. The security PIN remains stored and will not be erased.</p>
<pre>wm.reset();</pre>

<hr>

<h3>Configuration Functions</h3>

<h4><code>setTimeout()</code></h4>
<p>Set the portal config timeout in seconds. Default is 180 seconds (3 minutes). The timeout only applies if WiFi has never been configured before. Accepts a value of type <code>uint32_t</code>.</p>
<pre>wm.setTimeout(300);  // 5 minutes timeout</pre>

<h4><code>setIP()</code></h4>
<p>Set a custom IP address for the Access Point. Useful if you want to use an IP address other than the default (192.168.4.1). Requires three parameters: IP address, gateway, and subnet mask.</p>
<pre>wm.setIP(
  IPAddress(192, 168, 10, 1),
  IPAddress(192, 168, 10, 1),
  IPAddress(255, 255, 255, 0)
);</pre>

<h4><code>setDebug()</code></h4>
<p>Enable or disable debug logging to Serial Monitor. Very useful for troubleshooting connection problems. Accepts a boolean value of <code>true</code> or <code>false</code>.</p>
<pre>wm.setDebug(true);</pre>

<h4><code>autoReconnect()</code></h4>
<p>Enable or disable the auto-reconnect feature. If enabled, the ESP will attempt to reconnect every 1 second when the WiFi connection is lost. Default: true. Accepts a boolean value.</p>
<pre>wm.autoReconnect(true);</pre>

<hr>

<h3>Status & Information Functions</h3>

<h4><code>isConnected()</code></h4>
<p>Check if the ESP is connected to WiFi. Returns <code>true</code> if connected, <code>false</code> if not.</p>
<pre>if (wm.isConnected()) {
  Serial.println("WiFi OK");
}</pre>

<h4><code>getSSID()</code></h4>
<p>Get the name of the currently connected or stored WiFi SSID. Returns a value of type <code>String</code>.</p>
<pre>String ssid = wm.getSSID();
Serial.println(ssid);</pre>

<h4><code>getIP()</code></h4>
<p>Get the IP address of the ESP currently connected to the WiFi network. Returns an <code>IPAddress</code> object.</p>
<pre>IPAddress ip = wm.getIP();
Serial.println(ip);</pre>

<h4><code>printDebugInfo()</code></h4>
<p>Displays complete WiFi information to Serial Monitor, including: SSID, Channel, RSSI (signal strength), MAC Address, IP Address, Gateway, Netmask, DNS, and Free Heap. This function is only active when debug mode is enabled.</p>
<pre>wm.printDebugInfo();</pre>

<hr>

<h3>Callback Functions</h3>

<h4><code>onConnect()</code></h4>
<p>Registers a callback function to be called when Wi-Fi is successfully connected. Suitable for initializing services that require an internet connection, such as NTP, MQTT, or a web server. The callback function takes no parameters and returns no value.</p>
<pre>void onWiFiConnect() {
  Serial.println("Connected!");
}

wm.onConnect(onWiFiConnect);</pre>

<h4><code>onConfigPortalStart()</code></h4>
<p>Registers a callback function that will be called when the portal configuration starts. Useful for providing visual notifications such as LEDs or buzzers. The callback function takes no parameters and returns no value.</p>
<pre>void onPortalStart() {
  Serial.println("Portal opened");
}

wm.onConfigPortalStart(onPortalStart);</pre>

<h4><code>onDisconnect()</code></h4>
<p>Registers a callback function to be called when Wi-Fi disconnects from the network. Useful for error handling or resource cleanup. The callback function takes no parameters and returns no value.</p>
<pre>void onWiFiDisconnect() {
  Serial.println("WiFi lost!");
}

wm.onDisconnect(onWiFiDisconnect);</pre>

<hr>

<h3>Complete Example</h3>
<pre><code>#include "nuba_wm.h"

nuba::Manager wm;

void onConnect() {
  Serial.println("WiFi Connected!");
  wm.printDebugInfo();
}

void onDisconnect() {
  Serial.println("WiFi Disconnected!");
}

void setup() {
  Serial.begin(115200);
  
  wm.setDebug(true);
  wm.setTimeout(300);
  wm.autoReconnect(true);
  
  wm.onConnect(onConnect);
  wm.onDisconnect(onDisconnect);
  
  wm.begin("MyESP_AP", "password123");
}

void loop() {
  wm.run();
  delay(10);
}
</code></pre>

<hr>
