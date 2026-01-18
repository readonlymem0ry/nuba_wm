<h2>Functions</h2>

<h3>Main Functions</h3>
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
        <td><code>wm.begin();</code></td>
        <td>Start WiFi Manager</td>
    </tr>
    <tr>
        <td><code>begin(ssid, pass)</code></td>
        <td><code>const char*</code>, <code>const char*</code></td>
        <td><code>wm.begin("nuba AP", "ilovecoffee");</code></td>
        <td>Start softAP with custom credentials</td>
    </tr>
    <tr>
        <td><code>run()</code></td>
        <td>-</td>
        <td><code>wm.run();</code></td>
        <td>Run all process. Call in <code>loop()</code> section</td>
    </tr>
    <tr>
        <td><code>reset()</code></td>
        <td>-</td>
        <td><code>wm.reset();</code></td>
        <td>Reset WiFi credential then reboot to apply changes</td>
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
        <td><code>setDebug()</code></td>
        <td><code>bool</code></td>
        <td><code>wm.setDebug(true);</code></td>
        <td>Enable debug messages on Serial Monitor</td>
    </tr>
    <tr>
        <td><code>setTimeout(seconds)</code></td>
        <td><code>uint32_t</code></td>
        <td><code>wm.setTimeout(180);</code></td>
        <td>Portal timeout (default: 180s)</td>
    </tr>
    <tr>
        <td><code>setIP(ip, gatewway, netmask)</code></td>
        <td><code>IPAddress</code>, <code>IPAddress</code>, <code>IPAddress</code></td>
        <td><code>wm.setIP(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));</code></td>
        <td>Set softAP static IP</td>
    </tr>
    <tr>
        <td><code>autoReconnect(enable)</code></td>
        <td><code>bool</code></td>
        <td><code>wm.autoReconnect(true);</code></td>
        <td>Enable or disable auto reconnect feature</td>
    </tr>
    <tr>
        <td><code>reconnectTimeout(seconds)</code></td>
        <td><code>uint32_t</code></td>
        <td><code>wm.reconnectTimeout(120);</code></td>
        <td>Reconnect timeout (default: 60s)</td>
    </tr>
</table>

<h3>Status Checking</h3>
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
        <td><code>if (wm.isConnected()) { }</code></td>
        <td>Check WiFi status</td>
    </tr>
    <tr>
        <td><code>getSSID()</code></td>
        <td><code>String</code></td>
        <td><code>String ssid = wm.getSSID();</code></td>
        <td>Get current connected WiFi SSID</td>
    </tr>
    <tr>
        <td><code>getIP()</code></td>
        <td><code>IPAddress</code></td>
        <td><code>IPAddress ip = wm.getIP();</code></td>
        <td>Get current connected WiFi IP address</td>
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
        <td><code>wm.onConnect([]() { Serial.println("connected"); });</code></td>
        <td>Call function when connected</td>
    </tr>
    <tr>
        <td><code>onDisconnect(callback)</code></td>
        <td><code>void (*)()</code></td>
        <td><code>wm.onDisconnect([]() { Serial.println("disconnected"); });</code></td>
        <td>Call function when disconnected</td>
    </tr>
    <tr>
        <td><code>onConfigPortalStart(callback)</code></td>
        <td><code>void (*)()</code></td>
        <td><code>wm.onConfigPortalStart([]() { Serial.println("Portal"); });</code></td>
        <td>Call function when portal started</td>
    </tr>
</table>

<h3>Examples</h3>

<p><strong>Minimal:</strong></p>
<pre><code>#include "nuba_wm.h"
nuba::Manager wm; 

void setup() {
  wm.begin();
}

void loop() {
  wm.run();
}</code></pre>

<p><strong>Complete:</strong></p>
<pre><code>#include "nuba_wm.h"
nuba::Manager wm;

void setup() {
  Serial.begin(115200);
  
  wm.setDebug(true);
  wm.autoReconnect(true);
  wm.reconnectTimeout(120);
  
  wm.onConnect([]() {
    Serial.println("Connected!");
  });
  
  wm.begin("nuba AP", "ilovecoffee");
}

void loop() {
  wifi.run();
}</code></pre>
