# Praktikum Modul 3 - Protokol Komunikasi

## 1. Penjelasan Singkat Percobaan
Pada praktikum ini dilakukan dua percobaan, percobaan pertama mengimplementasikan pengiriman data dari ESP8266 ke server menggunakan protokol HTTP dengan metode POST dalam format JSON. Percobaan kedua mengimplementasikan pertukaran data dari ESP8266 ke broker MQTT menggunakan pola publish-subscribe dengan format data JSON.

## 2. Library yang digunakan
- ESP8266WiFi.h
- ArduinoJson.h
- ESP8266HTTPClient.h (untuk percobaan 3A dan program modifikasi)
- WiFiClientSecure.h (untuk percobaan 3A dan program modifikasi)
- PubSubClient.h (untuk percobaan 3B)

## 3. Penjelasan kode 

### Percobaan 3A

Percobaan 3A mengimplementasikan pengiriman data sensor dari ESP8266 ke server menggunakan protokol **HTTP POST** dengan format data **JSON** melalui koneksi **HTTPS (SSL/TLS)**.

#### Library yang Digunakan
- `ESP8266WiFi.h` — mengelola koneksi WiFi pada ESP8266.
- `ESP8266HTTPClient.h` — menyediakan fungsi HTTP client untuk mengirim request ke server.
- `WiFiClientSecure.h` — menangani koneksi HTTPS (enkripsi SSL/TLS).
- `ArduinoJson.h` — membuat dan mem-parsing data dalam format JSON.

#### Konfigurasi Awal
```cpp
const char* ssid = "S24";
const char* password = "11111111";
const char* serverUrl = "https://httpbin.org/post";
```
Bagian ini mendefinisikan kredensial WiFi (SSID dan password) serta URL server tujuan pengiriman data. URL `https://httpbin.org/post` adalah endpoint publik yang digunakan untuk menguji HTTP POST request.

#### Fungsi `setup()`
```cpp
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi berhasil terhubung!");
  Serial.println(WiFi.localIP());
}
```
- `Serial.begin(115200)` — menginisialisasi komunikasi serial untuk debugging dengan baud rate 115200.
- `WiFi.begin(ssid, password)` — memulai proses koneksi ke jaringan WiFi.
- Loop `while (WiFi.status() != WL_CONNECTED)` — menunggu hingga koneksi WiFi berhasil, mencetak titik (`.`) setiap 500ms sebagai indikator proses.
- Setelah terhubung, IP lokal ESP8266 ditampilkan di Serial Monitor.

#### Fungsi `loop()`
```cpp
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");

    JsonDocument doc;
    doc["suhu"] = 28.5;
    doc["kelembaban"] = 65.0;

    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0) {
      Serial.println(httpResponseCode);
      Serial.println(http.getString());
    } else {
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  delay(10000);
}
```
- `WiFi.status() == WL_CONNECTED` — memastikan WiFi masih terhubung sebelum mengirim data.
- `WiFiClientSecure client` + `client.setInsecure()` — membuat objek klien HTTPS dan menonaktifkan verifikasi sertifikat SSL (mode *insecure*) agar ESP8266 dapat terhubung ke server HTTPS tanpa memvalidasi sertifikat.
- `http.begin(client, serverUrl)` — menginisialisasi koneksi HTTP ke URL server yang ditentukan.
- `http.addHeader("Content-Type", "application/json")` — menambahkan header HTTP yang memberitahu server bahwa data yang dikirim berformat JSON.
- `JsonDocument doc` — membuat dokumen JSON, lalu mengisi field `suhu` dengan nilai `28.5` dan `kelembaban` dengan nilai `65.0` sebagai simulasi data sensor.
- `serializeJson(doc, requestBody)` — mengonversi objek JSON menjadi string, misalnya: `{"suhu":28.5,"kelembaban":65.0}`.
- `http.POST(requestBody)` — mengirim data ke server menggunakan metode HTTP POST dan mengembalikan kode response (misal `200` jika berhasil).
- Jika kode response bernilai positif, response dari server ditampilkan. Jika negatif, ditampilkan kode error.
- `http.end()` — menutup koneksi HTTP untuk membebaskan resource.
- `delay(10000)` — menunggu 10 detik sebelum pengiriman data berikutnya.


### Percobaan 3B

Percobaan 3B mengimplementasikan pengiriman data sensor dari ESP8266 ke **broker MQTT** menggunakan pola **publish-subscribe**. Data dikirim ke topic tertentu dalam format JSON setiap 5 detik sekali.

#### Library yang Digunakan
- `ESP8266WiFi.h` — mengelola koneksi WiFi pada ESP8266.
- `PubSubClient.h` — menyediakan fungsi MQTT client untuk publish dan subscribe ke broker.
- `ArduinoJson.h` — membuat dan mem-parsing data dalam format JSON.

#### Konfigurasi Awal
```cpp
const char* ssid = "S24";
const char* password = "11111111";

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "wizz/tk245004/kelompokTerakhir/sensor";

WiFiClient espClient;
PubSubClient client(espClient);
```
- `mqttServer` — alamat broker MQTT publik yang digunakan (`broker.hivemq.com`).
- `mqttPort` — port standar MQTT tanpa enkripsi, yaitu `1883`.
- `mqttTopic` — nama topic MQTT tujuan publikasi data sensor. Topic bersifat hierarkis (dipisah `/`) untuk memudahkan pengelompokan.
- `WiFiClient espClient` — objek koneksi TCP biasa (non-HTTPS).
- `PubSubClient client(espClient)` — objek MQTT client yang menggunakan `espClient` sebagai transport layer.

#### Fungsi `hubungkanWiFi()`
```cpp
void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi berhasil terhubung!");
  Serial.println(WiFi.localIP());
}
```
Fungsi ini menangani proses koneksi ke jaringan WiFi. Loop `while` akan terus berjalan hingga ESP8266 berhasil terhubung, lalu mencetak alamat IP yang diperoleh.

#### Fungsi `hubungkanMQTT()`
```cpp
void hubungkanMQTT() {
  while (!client.connected()) {
    String clientId = "ESP8266Client-";
    clientId += String(ESP.getChipId(), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("berhasil terhubung!");
    } else {
      Serial.print("gagal, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}
```
- `client.connected()` — mengecek apakah koneksi ke broker MQTT masih aktif.
- `ESP.getChipId()` — membaca ID unik chip ESP8266, dikonversi ke format HEX dan digabungkan ke string `clientId`. Ini memastikan setiap perangkat memiliki Client ID yang unik, mencegah konflik di broker.
- `client.connect(clientId.c_str())` — mencoba membuka koneksi ke broker MQTT. Jika gagal, `client.state()` mengembalikan kode error dan percobaan diulang setelah 2 detik.

#### Fungsi `setup()`
```cpp
void setup() {
  Serial.begin(115200);
  hubungkanWiFi();
  client.setServer(mqttServer, mqttPort);
}
```
- Menginisialisasi serial, memanggil `hubungkanWiFi()`, lalu `client.setServer()` untuk mengatur alamat dan port broker MQTT yang akan dituju.

#### Fungsi `loop()`
```cpp
void loop() {
  if (!client.connected()) {
    hubungkanMQTT();
  }
  client.loop();

  JsonDocument doc;
  doc["suhu"] = 28.5;
  doc["kelembaban"] = 65.0;

  char buffer[128];
  serializeJson(doc, buffer);

  bool berhasil = client.publish(mqttTopic, buffer);

  if (berhasil) {
    Serial.println(buffer);
  } else {
    Serial.println("Gagal mengirim data!");
  }

  delay(5000);
}
```
- `if (!client.connected())` — memeriksa koneksi MQTT setiap iterasi; jika terputus, akan memanggil `hubungkanMQTT()` untuk menyambung kembali secara otomatis.
- `client.loop()` — **wajib dipanggil secara rutin** agar library PubSubClient dapat memproses paket MQTT yang masuk/keluar dan menjaga koneksi tetap hidup (keepalive).
- `serializeJson(doc, buffer)` — menyerialisasi JSON ke dalam array karakter `buffer` berukuran 128 byte, contoh hasil: `{"suhu":28.5,"kelembaban":65.0}`.
- `client.publish(mqttTopic, buffer)` — mempublikasikan data ke topic MQTT. Mengembalikan `true` jika berhasil, `false` jika gagal.
- `delay(5000)` — menunggu 5 detik sebelum pengiriman data berikutnya.


### Modifikasi Percobaan 3A

Modifikasi pada percobaan 3A menambahkan **field timestamp** ke dalam data JSON yang dikirim, sehingga server dapat mengetahui kapan data tersebut dihasilkan. Selain penambahan ini, seluruh bagian kode lainnya identik dengan percobaan 3A.

#### Kode yang Ditambahkan

Penambahan dilakukan di dalam fungsi `loop()`, setelah field `suhu` dan `kelembaban` diisi:

```cpp
// Sebelum modifikasi (percobaan 3A):
doc["suhu"] = 28.5;
doc["kelembaban"] = 65.0;

// Setelah modifikasi (ditambahkan):
unsigned long waktuMillis = millis();
doc["waktu_ms"] = waktuMillis;
```

**Penjelasan baris yang ditambahkan:**
- `unsigned long waktuMillis = millis()` — mengambil nilai waktu dalam satuan **milidetik** sejak ESP8266 pertama kali dinyalakan. Tipe `unsigned long` digunakan karena nilai `millis()` dapat tumbuh hingga ~49 hari sebelum overflow.
- `doc["waktu_ms"] = waktuMillis` — menyisipkan nilai waktu tersebut ke dalam dokumen JSON dengan kunci `"waktu_ms"`.

**Contoh hasil JSON yang dikirim setelah modifikasi:**
```json
{"suhu":28.5,"kelembaban":65.0,"waktu_ms":15023}
```

Dengan tambahan field ini, setiap data yang diterima server memiliki informasi waktu relatif kapan data tersebut dibuat, berguna untuk analisis urutan atau latensi pengiriman.

## 4. Hasil Pengamatan

### Hasil Pengiriman Data melalui HTTP POST 

| No. | Waktu Pengiriman (s) | Data JSON yang Dikirim | HTTP Response Code | Response Body | Status Pengiriman |
| :---: | :---: | :--- | :---: | :--- | :---: |
| 1 | 0 | `{"suhu":28.5,"kelembaban":65}` | 200 | `"{"suhu":28.5,"kelembaban":65}"` | Berhasil |
| 2 | 10 | `{"suhu":28.5,"kelembaban":65}` | 200 | `"{"suhu":28.5,"kelembaban":65}"` | Berhasil |
| 3 | 20 | `{"suhu":28.5,"kelembaban":65}` | 200 | `"{"suhu":28.5,"kelembaban":65}"` | Berhasil |
| 4 | 30 | `{"suhu":28.5,"kelembaban":65}` | 200 | `"{"suhu":28.5,"kelembaban":65}"` | Berhasil |
| 5 | 40 | `{"suhu":28.5,"kelembaban":65}` | 200 | `"{"suhu":28.5,"kelembaban":65}"` | Berhasil |
| 6 | 50 | `{"suhu":28.5,"kelembaban":65}` | 200 | `"{"suhu":28.5,"kelembaban":65}"` | Berhasil |
| 7 | 60 | `{"suhu":28.5,"kelembaban":65}` | 200 | `"{"suhu":28.5,"kelembaban":65}"` | Berhasil |
| 8 | 70 | `{"suhu":28.5,"kelembaban":65}` | 200 | `"{"suhu":28.5,"kelembaban":65}"` | Berhasil |
| 9 | 80 | `{"suhu":28.5,"kelembaban":65}` | 200 | `"{"suhu":28.5,"kelembaban":65}"` | Berhasil |
| 10 | 90 | `{"suhu":28.5,"kelembaban":65}` | 200 | `"{"suhu":28.5,"kelembaban":65}"` | Berhasil |


### Hasil Komunikasi Data melalui MQTT

| No. | Waktu Pengiriman (s) | Status Broker | Topic | Data JSON yang Dipublish | Data Diterima Subscriber | Status Pengiriman |
| :---: | :---: | :---: | :--- | :--- | :--- | :---: |
| 1 | 0 | Terhubung | `wizz/esp8266/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 2 | 10 | Terhubung | `wizz/esp8266/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 3 | 20 | Terhubung | `wizz/esp8266/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 4 | 30 | Terhubung | `wizz/esp8266/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 5 | 40 | Terhubung | `wizz/esp8266/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 6 | 50 | Terhubung | `wizz/esp8266/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 7 | 60 | Terhubung | `wizz/esp8266/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 8 | 70 | Terhubung | `wizz/esp8266/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 9 | 80 | Terhubung | `wizz/esp8266/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 10 | 90 | Terhubung | `wizz/esp8266/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |

