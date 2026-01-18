<h1>nuba_wm</h1>

Simple WiFi manager with minimalist and modern UI for ESP32/ESP8266.
Developed to simplify your WiFi ESP configuration, features a minimalist yet modern and user-friendly UI, captive portal that redirects you to the portal page immediately after connecting to the ESP SoftAP. PIN verification makes it even more secure, so no one can configure your WiFi ESP without your knowledge.

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>nuba WiFi Manager - Function Reference</title>
    <style>
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
            line-height: 1.6;
            padding: 20px;
            max-width: 1200px;
            margin: 0 auto;
            background: #f6f8fa;
        }
        h1 {
            color: #24292e;
            border-bottom: 2px solid #e1e4e8;
            padding-bottom: 10px;
        }
        h2 {
            color: #24292e;
            margin-top: 30px;
            border-bottom: 1px solid #e1e4e8;
            padding-bottom: 5px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin: 20px 0;
            background: white;
            box-shadow: 0 1px 3px rgba(0,0,0,0.1);
        }
        th {
            background: #0366d6;
            color: white;
            padding: 12px;
            text-align: left;
            font-weight: 600;
        }
        td {
            padding: 12px;
            border-bottom: 1px solid #e1e4e8;
        }
        tr:hover {
            background: #f6f8fa;
        }
        code {
            background: #f6f8fa;
            padding: 2px 6px;
            border-radius: 3px;
            font-family: 'Courier New', monospace;
            color: #e83e8c;
            font-size: 13px;
        }
        .badge {
            display: inline-block;
            padding: 3px 8px;
            border-radius: 3px;
            font-size: 11px;
            font-weight: 600;
            margin-right: 5px;
        }
        .badge-required {
            background: #d73a49;
            color: white;
        }
        .badge-optional {
            background: #28a745;
            color: white;
        }
        .badge-callback {
            background: #6f42c1;
            color: white;
        }
        .note {
            background: #fff3cd;
            border-left: 4px solid #ffc107;
            padding: 10px;
            margin: 10px 0;
        }
    </style>
</head>
<body>

<h1>📚 nuba WiFi Manager - Function Reference</h1>

<h2>🚀 Core Functions</h2>
<table>
    <thead>
        <tr>
            <th style="width: 25%">Function</th>
            <th style="width: 20%">Parameters</th>
            <th style="width: 35%">Example</th>
            <th style="width: 20%">Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>begin()</code> <span class="badge badge-required">REQUIRED</span></td>
            <td>-</td>
            <td><code>wifi.begin();</code></td>
            <td>Initialize WiFi Manager with default settings</td>
        </tr>
        <tr>
            <td><code>begin(ssid)</code></td>
            <td><code>const char*</code></td>
            <td><code>wifi.begin("My AP");</code></td>
            <td>Initialize with custom AP SSID</td>
        </tr>
        <tr>
            <td><code>begin(ssid, password)</code></td>
            <td><code>const char*</code>, <code>const char*</code></td>
            <td><code>wifi.begin("My AP", "12345678");</code></td>
            <td>Initialize with custom AP SSID and password</td>
        </tr>
        <tr>
            <td><code>run()</code> <span class="badge badge-required">REQUIRED</span></td>
            <td>-</td>
            <td><code>wifi.run();</code></td>
            <td>Main loop handler - must be called in <code>loop()</code></td>
        </tr>
        <tr>
            <td><code>reset()</code></td>
            <td>-</td>
            <td><code>wifi.reset();</code></td>
            <td>Erase WiFi credentials and restart ESP</td>
        </tr>
    </tbody>
</table>

<h2>⚙️ Configuration Functions</h2>
<table>
    <thead>
        <tr>
            <th style="width: 25%">Function</th>
            <th style="width: 20%">Parameters</th>
            <th style="width: 35%">Example</th>
            <th style="width: 20%">Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>setDebug(enable)</code></td>
            <td><code>bool</code></td>
            <td><code>wifi.setDebug(true);</code><br><code>wifi.setDebug(false);</code></td>
            <td>Enable/disable debug mode</td>
        </tr>
        <tr>
            <td><code>setTimeout(seconds)</code></td>
            <td><code>uint32_t</code></td>
            <td><code>wifi.setTimeout(180);</code><br><code>wifi.setTimeout(300);</code></td>
            <td>Set config portal timeout (default: 180s)</td>
        </tr>
        <tr>
            <td><code>setIP(ip, gateway, subnet)</code></td>
            <td><code>IPAddress</code> × 3</td>
            <td><code>wifi.setIP(<br>&nbsp;&nbsp;IPAddress(192,168,1,1),<br>&nbsp;&nbsp;IPAddress(192,168,1,1),<br>&nbsp;&nbsp;IPAddress(255,255,255,0)<br>);</code></td>
            <td>Set static IP for AP mode</td>
        </tr>
        <tr>
            <td><code>setConfig(config)</code></td>
            <td><code>Config&amp;</code></td>
            <td><code>nuba::Config cfg;<br>strcpy(cfg.ssid, "AP");<br>wifi.setConfig(cfg);</code></td>
            <td>Set full AP configuration</td>
        </tr>
    </tbody>
</table>

<h2>🔄 Auto Reconnect Functions</h2>
<table>
    <thead>
        <tr>
            <th style="width: 25%">Function</th>
            <th style="width: 20%">Parameters</th>
            <th style="width: 35%">Example</th>
            <th style="width: 20%">Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>autoReconnect(enable)</code></td>
            <td><code>bool</code></td>
            <td><code>wifi.autoReconnect(true);</code><br><code>wifi.autoReconnect(false);</code></td>
            <td>Enable/disable auto reconnect (default: true)</td>
        </tr>
        <tr>
            <td><code>reconnectTimeout(seconds)</code></td>
            <td><code>uint32_t</code></td>
            <td><code>wifi.reconnectTimeout(30);</code><br><code>wifi.reconnectTimeout(120);</code><br><code>wifi.reconnectTimeout(300);</code></td>
            <td>Set reconnect timeout before opening AP (default: 60s)<br><br><strong>Auto Interval:</strong><br>≤30s → 3s interval<br>≤60s → 5s interval<br>≤120s → 7s interval<br>&gt;120s → 10s interval</td>
        </tr>
    </tbody>
</table>

<h2>📊 Status Functions</h2>
<table>
    <thead>
        <tr>
            <th style="width: 25%">Function</th>
            <th style="width: 20%">Return Type</th>
            <th style="width: 35%">Example</th>
            <th style="width: 20%">Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>isConnected()</code></td>
            <td><code>bool</code></td>
            <td><code>if (wifi.isConnected()) {<br>&nbsp;&nbsp;Serial.println("OK");<br>}</code></td>
            <td>Check if WiFi is connected</td>
        </tr>
        <tr>
            <td><code>getSSID()</code></td>
            <td><code>String</code></td>
            <td><code>String ssid = wifi.getSSID();<br>Serial.println(ssid);</code></td>
            <td>Get current WiFi SSID</td>
        </tr>
        <tr>
            <td><code>getIP()</code></td>
            <td><code>IPAddress</code></td>
            <td><code>IPAddress ip = wifi.getIP();<br>Serial.println(ip.toString());</code></td>
            <td>Get current IP address</td>
        </tr>
    </tbody>
</table>

<h2>🔔 Callback Functions</h2>
<table>
    <thead>
        <tr>
            <th style="width: 25%">Function</th>
            <th style="width: 20%">Parameter Type</th>
            <th style="width: 35%">Example</th>
            <th style="width: 20%">Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>onConnect(callback)</code> <span class="badge badge-callback">CALLBACK</span></td>
            <td><code>void (*func)()</code></td>
            <td><code>void onConnect() {<br>&nbsp;&nbsp;Serial.println("Connected!");<br>}<br>wifi.onConnect(onConnect);</code><br><br><strong>Lambda:</strong><br><code>wifi.onConnect([]() {<br>&nbsp;&nbsp;Serial.println("OK");<br>});</code></td>
            <td>Called when WiFi connects successfully</td>
        </tr>
        <tr>
            <td><code>onDisconnect(callback)</code> <span class="badge badge-callback">CALLBACK</span></td>
            <td><code>void (*func)()</code></td>
            <td><code>void onDisconnect() {<br>&nbsp;&nbsp;Serial.println("Lost!");<br>}<br>wifi.onDisconnect(onDisconnect);</code><br><br><strong>Lambda:</strong><br><code>wifi.onDisconnect([]() {<br>&nbsp;&nbsp;Serial.println("Disc");<br>});</code></td>
            <td>Called when WiFi disconnects</td>
        </tr>
        <tr>
            <td><code>onConfigPortalStart(callback)</code> <span class="badge badge-callback">CALLBACK</span></td>
            <td><code>void (*func)()</code></td>
            <td><code>void onPortal() {<br>&nbsp;&nbsp;Serial.println("Portal!");<br>}<br>wifi.onConfigPortalStart(onPortal);</code><br><br><strong>Lambda:</strong><br><code>wifi.onConfigPortalStart([]() {<br>&nbsp;&nbsp;Serial.println("AP");<br>});</code></td>
            <td>Called when config portal starts</td>
        </tr>
    </tbody>
</table>

<h2>📋 Config Structure Reference</h2>
<table>
    <thead>
        <tr>
            <th style="width: 25%">Field</th>
            <th style="width: 20%">Type</th>
            <th style="width: 35%">Example</th>
            <th style="width: 20%">Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>ssid</code></td>
            <td><code>char[32]</code></td>
            <td><code>strcpy(config.ssid, "My AP");</code></td>
            <td>AP SSID name (max 31 chars)</td>
        </tr>
        <tr>
            <td><code>password</code></td>
            <td><code>char[64]</code></td>
            <td><code>strcpy(config.password, "12345678");</code></td>
            <td>AP password (min 8 chars or empty)</td>
        </tr>
        <tr>
            <td><code>channel</code></td>
            <td><code>uint8_t</code></td>
            <td><code>config.channel = 6;</code></td>
            <td>WiFi channel (1-13, recommended: 1, 6, 11)</td>
        </tr>
        <tr>
            <td><code>hidden</code></td>
            <td><code>bool</code></td>
            <td><code>config.hidden = false;</code></td>
            <td>Hide SSID broadcast</td>
        </tr>
        <tr>
            <td><code>timeout</code></td>
            <td><code>uint32_t</code></td>
            <td><code>config.timeout = 180000;</code></td>
            <td>Portal timeout in milliseconds</td>
        </tr>
        <tr>
            <td><code>apIP</code></td>
            <td><code>IPAddress</code></td>
            <td><code>config.apIP = IPAddress(192,168,4,1);</code></td>
            <td>AP IP address</td>
        </tr>
        <tr>
            <td><code>gateway</code></td>
            <td><code>IPAddress</code></td>
            <td><code>config.gateway = IPAddress(192,168,4,1);</code></td>
            <td>AP gateway address</td>
        </tr>
        <tr>
            <td><code>subnet</code></td>
            <td><code>IPAddress</code></td>
            <td><code>config.subnet = IPAddress(255,255,255,0);</code></td>
            <td>AP subnet mask</td>
        </tr>
    </tbody>
</table>

<h2>💡 Quick Examples</h2>

<h3>Minimal Setup</h3>
<pre><code>#include "nuba_wm.h"

nuba::Manager wifi;

void setup() {
  Serial.begin(115200);
  wifi.begin();
}

void loop() {
  wifi.run();
}</code></pre>

<h3>With Callbacks</h3>
<pre><code>#include "nuba_wm.h"

nuba::Manager wifi;

void setup() {
  Serial.begin(115200);
  
  wifi.setDebug(true);
  wifi.onConnect([]() {
    Serial.println("Connected: " + wifi.getSSID());
  });
  
  wifi.begin("My AP", "12345678");
}

void loop() {
  wifi.run();
}</code></pre>

<h3>Production Ready</h3>
<pre><code>#include "nuba_wm.h"

nuba::Manager wifi;

void setup() {
  Serial.begin(115200);
  
  wifi.setDebug(false);
  wifi.autoReconnect(true);
  wifi.reconnectTimeout(300);  // 5 minutes
  wifi.setTimeout(600);         // 10 minutes
  
  wifi.onConnect([]() {
    digitalWrite(LED_BUILTIN, HIGH);
  });
  
  wifi.onDisconnect([]() {
    digitalWrite(LED_BUILTIN, LOW);
  });
  
  wifi.begin("Device Setup", "secure2024");
}

void loop() {
  wifi.run();
  // Your code here...
}</code></pre>

<div class="note">
  <strong>⚠️ Important Notes:</strong>
  <ul>
    <li><code>begin()</code> and <code>run()</code> are <strong>required</strong></li>
    <li><code>run()</code> must be called in <code>loop()</code> continuously</li>
    <li>AP password must be at least 8 characters or empty (open network)</li>
    <li>Credentials are stored in flash memory and persist after restart</li>
    <li>Default PIN is <code>0000</code> - users should update it via portal</li>
  </ul>
</div>

</body>
</html>
