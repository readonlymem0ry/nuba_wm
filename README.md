<h2>Function Reference</h2>
<p><em>A secure and simple WiFi manager library for ESP8266/ESP32</em></p>

<h3>Main Functions</h3>
<table>
  <thead>
    <tr>
      <th>Function</th>
      <th>Parameters</th>
      <th>Example</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>begin()</code></td>
      <td>
        <strong>apSSID</strong>: <code>const char*</code><br>
        <strong>apPassword</strong>: <code>const char*</code><br>
        <em>(both optional)</em>
      </td>
      <td>
        <pre>wm.begin();
wm.begin("nuba AP");
wm.begin("nuba AP", "ilovecoffee");</pre>
      </td>
      <td>Initialize WiFi Manager. If there's a saved WiFi connection, it will automatically connect. If it fails or doesn't exist, it will open the configuration portal.</td>
    </tr>
    <tr>
      <td><code>init()</code></td>
      <td>
        <strong>apSSID</strong>: <code>const char*</code><br>
        <strong>apPassword</strong>: <code>const char*</code><br>
        <em>(both optional)</em>
      </td>
      <td>
        <pre>wm.init();
wm.init("nuba AP", "ilovecoffee");</pre>
      </td>
      <td>Alias ​​of <code>begin()</code>. Same function, can use either one.</td>
    </tr>
    <tr>
      <td><code>run()</code></td>
      <td><code>void</code></td>
      <td>
        <pre>void loop() {
  wm.run();
}</pre>
      </td>
      <td>Handle web server, DNS server, dan auto-reconnect. <strong>Must be called in loop()</strong>.</td>
    </tr>
    <tr>
      <td><code>startConfigPortal()</code></td>
      <td>
        <strong>apSSID</strong>: <code>const char*</code><br>
        <strong>apPassword</strong>: <code>const char*</code><br>
        <em>(both optional)</em>
      </td>
      <td>
        <pre>wm.startConfigPortal();
wm.startConfigPortal("nuba AP");</pre>
      </td>
      <td>Force open the config portal in AP mode. Useful for reconfiguring WiFi via physical buttons.</td>
    </tr>
    <tr>
      <td><code>reset()</code></td>
      <td><code>void</code></td>
      <td>
        <pre>wm.reset();</pre>
      </td>
      <td>CLear the saved WiFi credentials and reboot the ESP. The PIN remains saved.</td>
    </tr>
  </tbody>
</table>

<h3>Configuration Functions</h3>
<table>
  <thead>
    <tr>
      <th>Function</th>
      <th>Parameters</th>
      <th>Example</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>setTimeout()</code></td>
      <td><strong>seconds</strong>: <code>uint32_t</code></td>
      <td>
        <pre>wm.setTimeout(300);
// 300 seconds timeout</pre>
      </td>
      <td>Sets the config portal timeout in seconds. Default is 180 seconds (3 minutes). Only active when WiFi has never been configured.</td>
    </tr>
    <tr>
      <td><code>setIP()</code></td>
      <td>
        <strong>ip</strong>: <code>IPAddress</code><br>
        <strong>gateway</strong>: <code>IPAddress</code><br>
        <strong>subnet</strong>: <code>IPAddress</code>
      </td>
      <td>
        <pre>wm.setIP(
  IPAddress(192,168,10,1),
  IPAddress(192,168,10,1),
  IPAddress(255,255,255,0)
);</pre>
      </td>
      <td>Set custom IP address for Access Point. Default: 192.168.4.1</td>
    </tr>
    <tr>
      <td><code>setDebug()</code></td>
      <td><strong>enable</strong>: <code>bool</code></td>
      <td>
        <pre>wm.setDebug(true);</pre>
      </td>
      <td>Enable/disable debug logging to Serial Monitor. Useful for troubleshooting.</td>
    </tr>
    <tr>
      <td><code>autoReconnect()</code></td>
      <td><strong>enable</strong>: <code>bool</code></td>
      <td>
        <pre>wm.autoReconnect(true);</pre>
      </td>
      <td>Enable/disable auto-reconnect. If true, the ESP will try to reconnect every 1 second when disconnected. Default: true</td>
    </tr>
  </tbody>
</table>

<h3>Status & Information Functions</h3>
<table>
  <thead>
    <tr>
      <th>Function</th>
      <th>Return Type</th>
      <th>Example</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>isConnected()</code></td>
      <td><code>bool</code></td>
      <td>
        <pre>if (wm.isConnected()) {
  Serial.println("WiFi ok");
}</pre>
      </td>
      <td>Checks if the ESP is connected to WiFi. Returns <code>true</code> if connected, <code>false</code> otherwise.</td>
    </tr>
    <tr>
      <td><code>getSSID()</code></td>
      <td><code>String</code></td>
      <td>
        <pre>String ssid = wm.getSSID();
Serial.println(ssid);</pre>
      </td>
      <td>Get the name of the currently connected or saved WiFi SSID.</td>
    </tr>
    <tr>
      <td><code>getIP()</code></td>
      <td><code>IPAddress</code></td>
      <td>
        <pre>IPAddress ip = wm.getIP();
Serial.println(ip);</pre>
      </td>
      <td>Get the IP address of the ESP that is currently connected.</td>
    </tr>
    <tr>
      <td><code>printDebugInfo()</code></td>
      <td><code>void</code></td>
      <td>
        <pre>wm.printDebugInfo();</pre>
      </td>
      <td>Displays complete WiFi information: SSID, Channel, RSSI, MAC, IP, Gateway, Netmask, DNS, and Free Heap. Only active if debug mode is on.</td>
    </tr>
  </tbody>
</table>

<h3>Callback Functions</h3>
<table>
  <thead>
    <tr>
      <th>Function</th>
      <th>Parameters</th>
      <th>Example</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>onConnect()</code></td>
      <td><strong>func</strong>: <code>void (*)()</code></td>
      <td>
        <pre>void onWiFiConnect() {
  Serial.println("Connected!");
}

wm.onConnect(onWiFiConnect);</pre>
      </td>
      <td>Callback is called when WiFi is successfully connected. Suitable for initializations that require a connection (NTP, MQTT, etc.).</td>
    </tr>
    <tr>
      <td><code>onConfigPortalStart()</code></td>
      <td><strong>func</strong>: <code>void (*)()</code></td>
      <td>
        <pre>void onPortalStart() {
  Serial.println("Portal opened");
}

wm.onConfigPortalStart(onPortalStart);</pre>
      </td>
      <td>Callback called when the portal configuration starts. Useful for LED or buzzer notifications.</td>
    </tr>
    <tr>
      <td><code>onDisconnect()</code></td>
      <td><strong>func</strong>: <code>void (*)()</code></td>
      <td>
        <pre>void onWiFiDisconnect() {
  Serial.println("WiFi lost!");
}

wm.onDisconnect(onWiFiDisconnect);</pre>
      </td>
      <td>Callback called when WiFi is disconnected. Useful for error handling or cleanup.</td>
    </tr>
  </tbody>
</table>

<h3>Basic Usage</h3>
<pre>
    <code>
#include "nuba_wm.h"
&nbsp;&nbsp;        
nuba::Manager wm;
&nbsp;&nbsp;
void setup() {
  Serial.begin(115200);
  wm.begin();
}
&nbsp;&nbsp;
void loop() {
  wm.run();
}
    </code>
</pre>   

<h3>Complete Example</h3>
<pre><code>
#include "nuba_wm.h"
&nbsp;&nbsp;
nuba::Manager wm;
&nbsp;&nbsp;
void onConnect() {
  Serial.println("WiFi Connected");
  wm.printDebugInfo();
}
&nbsp;&nbsp;
void onDisconnect() {
  Serial.println("WiFi Disconnected");
}
&nbsp;&nbsp;
void setup() {
  Serial.begin(115200);
  Serial.println("\n=== nuba_wm Demo ===");
  &nbsp;&nbsp;
  // Configuration
  wm.setDebug(true);
  wm.setTimeout(300);
  wm.autoReconnect(true);
  &nbsp;&nbsp;
  // Callbacks
  wm.onConnect(onConnect);
  wm.onDisconnect(onDisconnect);
  &nbsp;&nbsp;
  // Start WiFi Manager
  wm.begin("MyESP_AP", "password123");
  &nbsp;&nbsp;
  Serial.println("Setup complete!");
}
&nbsp;&nbsp;
void loop() {
  wm.run();
  &nbsp;&nbsp;
  // Your code here
  delay(10);
}
</code></pre>
