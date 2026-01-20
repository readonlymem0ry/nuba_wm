<h2>nuba_wm Function Reference</h2>
<p><em>A secure and simple WiFi manager library for ESP8266/ESP32</em></p>

<hr>

<h3>Core Functions</h3>

<h4><code>begin()</code></h4>
<p>Inisialisasi WiFi Manager dengan opsi SSID dan password untuk Access Point. Jika ada WiFi tersimpan, akan otomatis connect. Jika gagal atau belum ada konfigurasi, akan membuka config portal. Kedua parameter bersifat opsional.</p>
<pre>wm.begin();
wm.begin("MyESP");
wm.begin("MyESP", "password123");</pre>

<h4><code>init()</code></h4>
<p>Alias dari <code>begin()</code>. Fungsi yang sama, bisa gunakan salah satu sesuai preferensi. Menerima parameter SSID dan password untuk Access Point (keduanya opsional).</p>
<pre>wm.init();
wm.init("MyESP", "password123");</pre>

<h4><code>run()</code></h4>
<p>Handle web server, DNS server, dan auto-reconnect. <strong>Wajib dipanggil di dalam fungsi loop()</strong> untuk menjaga WiFi Manager tetap berjalan dengan baik.</p>
<pre>void loop() {
  wm.run();
}</pre>

<h4><code>startConfigPortal()</code></h4>
<p>Paksa membuka config portal mode AP. Berguna untuk re-konfigurasi WiFi secara manual, misalnya menggunakan tombol fisik. Dapat menerima SSID dan password custom untuk Access Point (opsional).</p>
<pre>wm.startConfigPortal();
wm.startConfigPortal("ESP_Config");</pre>

<h4><code>reset()</code></h4>
<p>Menghapus WiFi credentials yang tersimpan dan restart ESP. PIN keamanan tetap tersimpan dan tidak akan terhapus.</p>
<pre>wm.reset();</pre>

<hr>

<h3>Configuration Functions</h3>

<h4><code>setTimeout()</code></h4>
<p>Mengatur timeout config portal dalam satuan detik. Default 180 detik (3 menit). Timeout hanya berlaku saat WiFi belum pernah dikonfigurasi sama sekali. Menerima nilai dalam tipe <code>uint32_t</code>.</p>
<pre>wm.setTimeout(300);  // 5 menit timeout</pre>

<h4><code>setIP()</code></h4>
<p>Mengatur custom IP address untuk Access Point. Berguna jika ingin menggunakan IP selain default (192.168.4.1). Memerlukan tiga parameter: IP address, gateway, dan subnet mask.</p>
<pre>wm.setIP(
  IPAddress(192, 168, 10, 1),
  IPAddress(192, 168, 10, 1),
  IPAddress(255, 255, 255, 0)
);</pre>

<h4><code>setDebug()</code></h4>
<p>Mengaktifkan atau menonaktifkan debug log ke Serial Monitor. Sangat berguna untuk troubleshooting masalah koneksi. Menerima nilai boolean <code>true</code> atau <code>false</code>.</p>
<pre>wm.setDebug(true);</pre>

<h4><code>autoReconnect()</code></h4>
<p>Mengaktifkan atau menonaktifkan fitur auto reconnect. Jika diaktifkan, ESP akan mencoba reconnect setiap 1 detik saat koneksi WiFi terputus. Default: true. Menerima nilai boolean.</p>
<pre>wm.autoReconnect(true);</pre>

<hr>

<h3>Status & Information Functions</h3>

<h4><code>isConnected()</code></h4>
<p>Mengecek apakah ESP terhubung ke WiFi. Mengembalikan <code>true</code> jika connected, <code>false</code> jika tidak.</p>
<pre>if (wm.isConnected()) {
  Serial.println("WiFi OK");
}</pre>

<h4><code>getSSID()</code></h4>
<p>Mengambil nama SSID WiFi yang sedang terhubung atau tersimpan di memori. Mengembalikan nilai bertipe <code>String</code>.</p>
<pre>String ssid = wm.getSSID();
Serial.println(ssid);</pre>

<h4><code>getIP()</code></h4>
<p>Mengambil IP address ESP yang sedang terhubung ke jaringan WiFi. Mengembalikan objek <code>IPAddress</code>.</p>
<pre>IPAddress ip = wm.getIP();
Serial.println(ip);</pre>

<h4><code>printDebugInfo()</code></h4>
<p>Menampilkan informasi lengkap WiFi ke Serial Monitor meliputi: SSID, Channel, RSSI (kekuatan sinyal), MAC Address, IP Address, Gateway, Netmask, DNS, dan Free Heap. Fungsi ini hanya aktif jika debug mode diaktifkan.</p>
<pre>wm.printDebugInfo();</pre>

<hr>

<h3>Callback Functions</h3>

<h4><code>onConnect()</code></h4>
<p>Mendaftarkan callback function yang akan dipanggil saat WiFi berhasil terhubung. Cocok untuk inisialisasi layanan yang membutuhkan koneksi internet seperti NTP, MQTT, atau web server. Callback function tidak menerima parameter dan tidak mengembalikan nilai.</p>
<pre>void onWiFiConnect() {
  Serial.println("Connected!");
}

wm.onConnect(onWiFiConnect);</pre>

<h4><code>onConfigPortalStart()</code></h4>
<p>Mendaftarkan callback function yang akan dipanggil saat config portal dimulai. Berguna untuk memberikan notifikasi visual seperti LED atau buzzer. Callback function tidak menerima parameter dan tidak mengembalikan nilai.</p>
<pre>void onPortalStart() {
  Serial.println("Portal opened");
}

wm.onConfigPortalStart(onPortalStart);</pre>

<h4><code>onDisconnect()</code></h4>
<p>Mendaftarkan callback function yang akan dipanggil saat WiFi terputus dari jaringan. Berguna untuk handling error atau cleanup resources. Callback function tidak menerima parameter dan tidak mengembalikan nilai.</p>
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

<h3>Important Notes</h3>
<ul>
  <li>Selalu panggil <code>wm.run()</code> di dalam fungsi <code>loop()</code></li>
  <li>PIN default adalah <strong>0000</strong>, segera ubah melalui web portal untuk keamanan</li>
  <li>Auto reconnect akan mencoba koneksi ulang setiap 1 detik saat WiFi terputus</li>
  <li>Timeout config portal hanya berlaku saat WiFi belum pernah dikonfigurasi</li>
  <li>Gunakan <code>printDebugInfo()</code> untuk troubleshooting masalah koneksi</li>
</ul>
