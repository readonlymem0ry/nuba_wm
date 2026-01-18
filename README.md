<h2>Functions</h2>

<h3>Core Functions</h3>
<table>
    <tr>
        <th>Function</th>
        <th>Parameter Data Types</th>
        <th>Example</th>
        <th>Description</th>
    </tr>
    <tr>
        <td><code>begin()</code></td>
        <td>-</td>
        <td><code>wifi.begin();</code></td>
        <td>Start WiFi Manager</td>
    </tr>
    <tr>
        <td><code>begin(ssid, pass)</code></td>
        <td><code>const char*</code>, <code>const char*</code></td>
        <td><code>wifi.begin("My AP", "12345678");</code></td>
        <td>Start with custom AP credentials</td>
    </tr>
    <tr>
        <td><code>run()</code></td>
        <td>-</td>
        <td><code>wifi.run();</code></td>
        <td>Main loop - call in <code>loop()</code></td>
    </tr>
    <tr>
        <td><code>reset()</code></td>
        <td>-</td>
        <td><code>wifi.reset();</code></td>
        <td>Reset WiFi and restart</td>
    </tr>
</table>

<h3>Configuration</h3>
<table>
    <tr>
        <th>Function</th>
        <th>Parameter Data Types</th>
        <th>Example</th>
        <th>Description</th>
    </tr>
    <tr>
        <td><code>setDebug(enable)</code></td>
        <td><code>bool</code></td>
        <td><code>wifi.setDebug(true);</code></td>
        <td>Enable debug messages</td>
    </tr>
    <tr>
        <td><code>setTimeout(seconds)</code></td>
        <td><code>uint32_t</code></td>
        <td><code>wifi.setTimeout(180);</code></td>
        <td>Portal timeout (default: 180s)</td>
    </tr>
    <tr>
        <td><code>setIP(ip, gw, subnet)</code></td>
        <td><code>IPAddress</code>, <code>IPAddress</code>, <code>IPAddress</code></td>
        <td><code>wifi.setIP(IPAddress(192,168,1,1), IPAddress(192,168,1,1), IPAddress(255,255,255,0));</code></td>
        <td>Set AP static IP</td>
    </tr>
    <tr>
        <td><code>setConfig(config)</code></td>
        <td><code>Config&amp;</code></td>
        <td><code>nuba::Config cfg;<br>wifi.setConfig(cfg);</code></td>
        <td>Set full AP configuration</td>
    </tr>
    <tr>
        <td><code>autoReconnect(enable)</code></td>
        <td><code>bool</code></td>
        <td><code>wifi.autoReconnect(true);</code></td>
        <td>Enable auto reconnect</td>
    </tr>
    <tr>
        <td><code>reconnectTimeout(seconds)</code></td>
        <td><code>uint32_t</code></td>
        <td><code>wifi.reconnectTimeout(120);</code></td>
        <td>Reconnect timeout (default: 60s)</td>
    </tr>
</table>

<h3>Status Check</h3>
<table>
    <tr>
        <th>Function</th>
        <th>Return Type</th>
        <th>Example</th>
        <th>Description</th>
    </tr>
    <tr>
        <td><code>isConnected()</code></td>
        <td><code>bool</code></td>
        <td><code>if (wifi.isConnected()) { }</code></td>
        <td>Check WiFi status</td>
    </tr>
    <tr>
        <td><code>getSSID()</code></td>
        <td><code>String</code></td>
        <td><code>String ssid = wifi.getSSID();</code></td>
        <td>Get current SSID</td>
    </tr>
    <tr>
        <td><code>getIP()</code></td>
        <td><code>IPAddress</code></td>
        <td><code>IPAddress ip = wifi.getIP();</code></td>
        <td>Get IP address</td>
    </tr>
</table>

<h3>Callbacks</h3>
<table>
    <tr>
        <th>Function</th>
        <th>Parameter Data Types</th>
        <th>Example</th>
        <th>Description</th>
    </tr>
    <tr>
        <td><code>onConnect(callback)</code></td>
        <td><code>void (*)()</code></td>
        <td><code>wifi.onConnect([]() { Serial.println("OK"); });</code></td>
        <td>Called when connected</td>
    </tr>
    <tr>
        <td><code>onDisconnect(callback)</code></td>
        <td><code>void (*)()</code></td>
        <td><code>wifi.onDisconnect([]() { Serial.println("Lost"); });</code></td>
        <td>Called when disconnected</td>
    </tr>
    <tr>
        <td><code>onConfigPortalStart(callback)</code></td>
        <td><code>void (*)()</code></td>
        <td><code>wifi.onConfigPortalStart([]() { Serial.println("Portal"); });</code></td>
        <td>Called when portal starts</td>
    </tr>
</table>

<h3>Examples</h3>

<p><strong>Minimal:</strong></p>
<pre><code>#include "nuba_wm.h"
nuba::Manager wifi;

void setup() {
  wifi.begin();
}

void loop() {
  wifi.run();
}</code></pre>

<p><strong>Complete:</strong></p>
<pre><code>#include "nuba_wm.h"
nuba::Manager wifi;

void setup() {
  Serial.begin(115200);
  
  wifi.setDebug(true);
  wifi.autoReconnect(true);
  wifi.reconnectTimeout(120);
  
  wifi.onConnect([]() {
    Serial.println("Connected!");
  });
  
  wifi.begin("My AP", "12345678");
}

void loop() {
  wifi.run();
}</code></pre>
