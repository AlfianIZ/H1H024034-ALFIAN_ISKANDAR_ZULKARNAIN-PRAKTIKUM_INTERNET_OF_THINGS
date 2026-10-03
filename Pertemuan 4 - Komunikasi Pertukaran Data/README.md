# Praktikum Modul 4 - Komunikasi Pertukaran Data

## 1. Penjelasan Singkat Percobaan
Pada praktikum modul 4 ini, dilakukan dua percobaan, percobaan pertama mengimplementasikan mekanisme subscribe pada MQTT beserta proses deserialisasi data JSON untuk mengendalikan aktuator berdasarkan perintah yang diterima. Percobaan kedua mengimplementasikan sistem IoT yang dapat mempublikasikan data sensor dan menerima perintah kendali secara bersamaan (full duplex)

## 2. Library yang Digunakan
- ESP8266WiFi.h
- PubSubClient.h
- ArduinoJson.h
- DHT.h (untuk percobaan 4B)

## 3. Penjelasan Kode
### Percobaan 4A

Percobaan 4A mengimplementasikan mekanisme **subscribe** pada protokol **MQTT** serta proses **deserialisasi payload JSON** pada NodeMCU ESP8266 untuk mengontrol aktuator (LED) berdasarkan pesan perintah yang diterima dari broker.


#### Konfigurasi Pin dan Parameter Jaringan
```cpp
const char* ssid = "S24";
const char* password = "11111111";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* topicPerintah = "unsoed/tk245004/panas";
const int ledPin = 5;

WiFiClient espClient;
PubSubClient client(espClient);
```
- `ssid` & `password` — kredensial access point WiFi untuk akses internet ESP8266.
- `mqttServer` — alamat broker MQTT publik yang dituju (`broker.hivemq.com`).
- `mqttPort` — port standar komunikasi MQTT tanpa enkripsi (port `1883`).
- `topicPerintah` — topic MQTT tempat ESP8266 akan berlangganan (subscribe) untuk menerima instruksi kendali.
- `ledPin` — pin GPIO yang digunakan untuk aktuator LED (GPIO 5 / pin D1 pada NodeMCU).
- `WiFiClient espClient` & `PubSubClient client(espClient)` — inisialisasi layer koneksi TCP dan klien MQTT.

#### Fungsi Callback (`callback`)
```cpp
void callback(char* topic, byte* payload, unsigned int length) {
  String pesan;
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }
  
  Serial.print("Pesan diterima [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(pesan);
  
  // Deserialisasi data JSON yang diterima
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, pesan);
  
  if (error) {
    Serial.print("Gagal parsing JSON: ");
    Serial.println(error.c_str());
    return;
  }
  
  const char* perintah = doc["perintah"];
  
  if (String(perintah) == "ON") {
    digitalWrite(ledPin, HIGH);
    Serial.println("Aktuator: ON");
  } else if (String(perintah) == "OFF") {
    digitalWrite(ledPin, LOW);
    Serial.println("Aktuator: OFF");
  }
}
```
- Fungsi ini merupakan *event handler* yang akan dipanggil secara otomatis oleh `PubSubClient` setiap kali ada pesan baru yang masuk pada topic yang di-subscribe.
- **Konversi Payload:** Loop `for` mengonversi array byte `payload` menjadi tipe data `String pesan`.
- **Deserialisasi JSON:** Objek `JsonDocument doc` disiapkan dan fungsi `deserializeJson(doc, pesan)` digunakan untuk mem-parsing payload JSON (misal: `{"perintah":"ON"}`).
- **Pengecekan Error:** Jika parsing gagal, pesan error dicetak ke Serial Monitor dan fungsi langsung dihentikan (`return;`).
- **Aksi Aktuator:** Nilai dari kunci `"perintah"` dibaca (`doc["perintah"]`). Jika bernilai `"ON"`, maka `digitalWrite(ledPin, HIGH)` menyalakan LED. Jika bernilai `"OFF"`, maka `digitalWrite(ledPin, LOW)` memadamkan LED.

#### Fungsi `hubungkanWiFi()`
```cpp
void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi berhasil terhubung!");
}
```
- Menghubungkan ESP8266 ke jaringan WiFi secara blocking hingga status koneksi bernilai `WL_CONNECTED`.

#### Fungsi `hubungkanMQTT()`
```cpp
void hubungkanMQTT() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke broker MQTT...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("berhasil terhubung!");
      client.subscribe(topicPerintah); // subscribe setelah berhasil terhubung
      Serial.print("Subscribe ke topic: ");
      Serial.println(topicPerintah);
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 2 detik");
      delay(2000);
    }
  }
}
```
- Melakukan perulangan hingga ESP8266 berhasil terhubung ke broker MQTT.
- `clientId` di-generate secara acak (`ESP8266Client-` + angka heksadesimal acak) untuk mencegah duplikasi Client ID pada broker publik.
- `client.connect(...)` membuka koneksi ke broker. Begitu terhubung, langsung memanggil `client.subscribe(topicPerintah)` agar ESP8266 mendengarkan instruksi pada topic tersebut. Jika gagal, koneksi dicoba kembali setelah 2 detik.

#### Fungsi `setup()`
```cpp
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  hubungkanWiFi();
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback); // daftarkan fungsi callback
}
```
- Menginisialisasi komunikasi Serial dengan baud rate 115200.
- Mengatur `ledPin` sebagai output dan kondisi awal mati (`LOW`).
- Memanggil `hubungkanWiFi()`.
- Menentukan alamat broker serta port melalui `client.setServer()`, lalu mendaftarkan fungsi penangan pesan masuk melalui `client.setCallback(callback)`.

#### Fungsi `loop()`
```cpp
void loop() {
  if (!client.connected()) {
    hubungkanMQTT();
  }
  client.loop(); // wajib dipanggil terus-menerus agar pesan dapat diterima
}
```
- Mengecek koneksi MQTT. Jika terputus, `hubungkanMQTT()` dipanggil kembali (auto-reconnect).
- `client.loop()` dipanggil secara kontinu untuk memproses buffer komunikasi MQTT, menangani *keep-alive ping*, dan memicu fungsi `callback` ketika ada pesan masuk.


### Percobaan 4B

Percobaan 4B mengimplementasikan komunikasi **dua arah (*full duplex*)** menggunakan protokol MQTT. NodeMCU ESP8266 secara bersamaan mempublikasikan (**publish**) data suhu dari sensor DHT11 ke broker secara berkala (*non-blocking*), sekaligus menerima (**subscribe**) perintah kendali untuk menyalakan/mematikan aktuator LED secara *real-time*.

#### Konfigurasi Pin, Sensor, dan Parameter Jaringan
```cpp
const char* ssid = "S24";
const char* password = "11111111";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

const char* topicData = "unsoed/panas/data";
const char* topicPerintah = "unsoed/panas/perintah";

#define DHTPIN 4
#define DHTTYPE DHT11

const int ledPin = 5;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long waktuTerakhirPublish = 0;
const long intervalPublish = 5000; // publish data setiap 5 detik (non-blocking)
```
- `topicData` — topic MQTT tujuan untuk mempublikasikan data pembacaan suhu.
- `topicPerintah` — topic MQTT tempat ESP8266 berlangganan (*subscribe*) guna menerima instruksi kontrol.
- `DHTPIN 4` & `DHTTYPE DHT11` — menentukan sensor DHT11 terhubung ke pin GPIO 4 (pin D2 pada NodeMCU).
- `ledPin = 5` — pin GPIO 5 (pin D1 pada NodeMCU) yang dihubungkan ke aktuator LED.
- `DHT dht(DHTPIN, DHTTYPE)` — membuat objek driver sensor DHT.
- `waktuTerakhirPublish` & `intervalPublish` — variabel penanda waktu berbasis `millis()` untuk mengatur interval pengiriman data setiap 5.000 ms (5 detik) secara *non-blocking* tanpa menggunakan `delay()`.

#### Fungsi Callback (`callback`)
```cpp
void callback(char* topic, byte* payload, unsigned int length) {
  String pesan;
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }
  
  JsonDocument doc;
  if (deserializeJson(doc, pesan)) return; // abaikan jika parsing gagal
  
  const char* perintah = doc["perintah"];
  digitalWrite(ledPin, String(perintah) == "ON" ? HIGH : LOW);
  
  Serial.print("Perintah diterima -> Aktuator: ");
  Serial.println(perintah);
}
```
- Berfungsi menangani setiap pesan yang masuk dari `topicPerintah`.
- Mengubah array byte `payload` menjadi `String pesan`.
- `deserializeJson(doc, pesan)` mem-parsing JSON perintah. Jika proses parsing gagal/error, eksekusi fungsi langsung dibatalkan (`return`).
- Mengekstrak nilai dari properti `"perintah"`, kemudian mengendalikan pin LED menggunakan operator *ternary*: jika bernilai `"ON"` maka pin diberi logika `HIGH`, selain itu diberi logika `LOW`.

#### Fungsi `hubungkanWiFi()`
```cpp
void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi berhasil terhubung!");
}
```
- Memulai dan menunggu sambungan WiFi hingga statusnya `WL_CONNECTED`.

#### Fungsi `hubungkanMQTT()`
```cpp
void hubungkanMQTT() {
  while (!client.connected()) {
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      client.subscribe(topicPerintah);
      Serial.println("Terhubung dan subscribe topic perintah");
    } else {
      delay(2000);
    }
  }
}
```
- Membuat Client ID acak dan menghubungkan ESP8266 ke broker MQTT.
- Begitu koneksi berhasil, ESP8266 langsung men-subscribe `topicPerintah` agar siap menerima data perintah kendali kapan saja.

#### Fungsi `setup()`
```cpp
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  dht.begin();
  
  hubungkanWiFi();
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}
```
- Mengatur komunikasi Serial (115200 baud), mode `ledPin` sebagai `OUTPUT`, dan menginisialisasi sensor suhu melalui `dht.begin()`.
- Memanggil fungsi koneksi WiFi, menyetel konfigurasi broker MQTT, serta mendaftarkan fungsi `callback`.

#### Fungsi `loop()`
```cpp
void loop() {
  if (!client.connected()) {
    hubungkanMQTT();
  }
  
  client.loop(); // memproses pesan masuk secara terus-menerus
  
  // Publish data sensor secara berkala tanpa memblokir proses subscribe
  if (millis() - waktuTerakhirPublish > intervalPublish) {
    waktuTerakhirPublish = millis();
    float suhu = dht.readTemperature();
    
    if (!isnan(suhu)) {
      JsonDocument doc;
      doc["suhu"] = suhu;
      
      char buffer[128];
      serializeJson(doc, buffer);
      client.publish(topicData, buffer);
      
      Serial.print("Data terkirim: ");
      Serial.println(buffer);
    } else {
      Serial.println("Gagal membaca sensor DHT!");
    }
  }
}
```
- `client.loop()` dipanggil di setiap perulangan untuk memeriksa dan memproses pesan masuk dari broker secepat mungkin.
- **Mekanisme Non-blocking Timing (`millis()`):** Pengiriman data berkala dilakukan menggunakan selisih `millis() - waktuTerakhirPublish > intervalPublish`. Pola ini menghindari penggunaan fungsi `delay()`, sehingga ESP8266 tetap responsif menerima perintah kendali kapan saja.
- `dht.readTemperature()` membaca nilai temperatur lingkungan. Nilai divalidasi dengan `!isnan(suhu)`.
- Data suhu dimasukkan ke dalam objek JSON `doc["suhu"] = suhu`, kemudian diserialisasi ke dalam array `buffer` dan dipublikasikan ke `topicData` via `client.publish()`.

### Modifikasi Percobaan 4A 

Modifikasi pada percobaan 4A menambahkan fitur **pengaturan tingkat kecerahan LED menggunakan sinyal PWM (Pulse Width Modulation)** berdasarkan payload JSON yang diterima, serta menambahkan **validasi keberadaan *key*** sebelum data diproses. Selain bagian-bagian tersebut, alur utama program tetap sama dengan percobaan 4A.

#### 1. Penambahan Konstanta PWM
```cpp
// Sebelum modifikasi:
// (belum ada deklarasi PWM)

// Setelah modifikasi (ditambahkan):
const int PWM_MAKS = 255;   // nilai maksimum resolusi PWM
```
- `PWM_MAKS` bernilai `255` didefinisikan sebagai batas atas rentang nilai duty cycle PWM (resolusi 8-bit, 0–255).

#### 2. Modifikasi pada Fungsi `callback()`
Pada percobaan 4A asli, LED hanya dikontrol secara digital (`HIGH`/`LOW`). Pada modifikasi ini, ditambahkan validasi field, pembacaan parameter `"intensitas"`, dan pengaturan kecerahan dengan `analogWrite()`:

```cpp
  const char* perintah = doc["perintah"];

  // --- DITAMBAHKAN: Validasi keberadaan key 'perintah' ---
  if (perintah == nullptr) {                                  
    Serial.println("Kunci 'perintah' tidak ditemukan");       
    return;                                                   
  }                                                           

  // --- DITAMBAHKAN: Pembacaan key 'intensitas' & pembatasan nilai ---
  int intensitas = doc["intensitas"] | PWM_MAKS;              
  intensitas = constrain(intensitas, 0, PWM_MAKS);            

  // --- DIMODIFIKASI: Menggunakan analogWrite menggantikan digitalWrite ---
  if (String(perintah) == "ON") {
    analogWrite(ledPin, intensitas);                          
    Serial.print("Aktuator: ON, intensitas: ");               
    Serial.println(intensitas);                               
  } else if (String(perintah) == "OFF") {
    analogWrite(ledPin, 0);                                   
    Serial.println("Aktuator: OFF");
  }
```

**Penjelasan kode yang ditambahkan/dimodifikasi:**
- **Validasi `perintah == nullptr`**: Mengecek apakah dokumen JSON benar-benar memuat kunci `"perintah"`. Jika pesan JSON tidak memuat kunci tersebut, program mencetak peringatan ke Serial Monitor dan langsung menghentikan fungsi (`return;`), mencegah akses pointer null.
- **Nilai *Default* dengan Fallback `|`**: Ekspresi `doc["intensitas"] | PWM_MAKS` membaca nilai kecerahan dari JSON. Jika pengirim tidak menyertakan kunci `"intensitas"`, nilainya secara otomatis akan memakai nilai *default* yaitu `PWM_MAKS` (255 / kecerahan maksimal).
- **Fungsi `constrain(intensitas, 0, PWM_MAKS)`**: Memastikan nilai intensitas yang masuk tetap berada dalam batas rentang aman `0` hingga `255`, mencegah nilai negatif atau melebihi batas.
- **`analogWrite(ledPin, intensitas)`**: Menggantikan `digitalWrite()`, sehingga saat perintah bernilai `"ON"`, tingkat kecerahan LED diatur sesuai nilai PWM yang diminta (misal: `{"perintah":"ON", "intensitas":128}`). Saat `"OFF"`, nilai duty cycle disetel ke `0` (LED padam total).

#### 3. Modifikasi pada Fungsi `setup()`
```cpp
// Sebelum modifikasi (percobaan 4A):
pinMode(ledPin, OUTPUT);
digitalWrite(ledPin, LOW);

// Setelah modifikasi (dimodifikasi):
pinMode(ledPin, OUTPUT);
analogWriteRange(PWM_MAKS);   // Menyetel rentang maksimum resolusi PWM ke 255
analogWrite(ledPin, 0);       // Menginisialisasi kondisi awal LED mati (duty cycle 0)
```
- `analogWriteRange(PWM_MAKS)` — secara *default* ESP8266 menggunakan rentang PWM 0–1023 (10-bit). Fungsi ini mengubah rentang skala PWM menjadi 0–255 agar kompatibel dengan standar nilai 8-bit yang dikirimkan.
- `analogWrite(ledPin, 0)` — memastikan output pin PWM bernilai 0 saat ESP8266 baru dinyalakan.


### Modifikasi Percobaan 4B

Modifikasi pada percobaan 4B menambahkan **aktuator kedua berupa Buzzer** beserta topic MQTT terpisah (`topicBuzzer`). Dengan modifikasi ini, ESP8266 dapat memilah instruksi kendali berdasarkan nama topic yang masuk (apakah perintah ditujukan untuk LED atau Buzzer). Selain bagian tersebut, logika pembacaan suhu DHT11 dan alur publikasi data tetap sama seperti percobaan 4B asli.

#### 1. Penambahan Topic MQTT dan Pin Buzzer
```cpp
// Sebelum modifikasi (percobaan 4B):
const char* topicData = "unsoed/panas/data";
const char* topicPerintah = "unsoed/panas/perintah";
const int ledPin = 5;

// Setelah modifikasi (ditambahkan):
const char* topicData = "unsoed/panas/data";
const char* topicPerintah = "unsoed/panas/perintah";
const char* topicBuzzer = "unsoed/panas/buzzer";    // Ditambahkan: Topic khusus buzzer
const int ledPin = 5;
const int buzzerPin = 14;                          // Ditambahkan: GPIO 14 (D5) untuk buzzer
```
- `topicBuzzer` — topic MQTT baru tempat ESP8266 menerima perintah kendali khusus untuk buzzer.
- `buzzerPin = 14` — mendefinisikan pin GPIO 14 (pin D5 pada NodeMCU) sebagai jalur kontrol aktuator buzzer.

#### 2. Modifikasi pada Fungsi `callback()`
Pada percobaan 4B asli, fungsi callback hanya mengasumsikan seluruh pesan masuk ditujukan ke LED. Pada modifikasi ini, ditambahkan validasi *null pointer* serta pemilahan topic menggunakan fungsi perbandingan string `strcmp()`:

```cpp
  const char* perintah = doc["perintah"];
  if (perintah == NULL) return; // Ditambahkan: cegah error jika key 'perintah' tidak ada

  // Dimodifikasi: Pengecekan topic asal pesan masuk
  if (strcmp(topic, topicPerintah) == 0) { 
    digitalWrite(ledPin, String(perintah) == "ON" ? HIGH : LOW);
    Serial.print("[LED] Perintah diterima -> Aktuator: ");
    Serial.println(perintah);
  } else if (strcmp(topic, topicBuzzer) == 0) {
    digitalWrite(buzzerPin, String(perintah) == "ON" ? HIGH : LOW); 
    Serial.print("[BUZZER] Perintah diterima -> Aktuator: ");
    Serial.println(perintah); 
  }
```

**Penjelasan kode yang ditambahkan/dimodifikasi:**
- **Validasi `perintah == NULL`**: Memastikan dokumen JSON valid dan memiliki properti `"perintah"` sebelum diproses.
- **Routing Topic dengan `strcmp()`**:
  - Jika pesan datang dari `topicPerintah` (`strcmp(...) == 0`), maka perintah `"ON"`/`"OFF"` diterapkan ke `ledPin`.
  - Jika pesan datang dari `topicBuzzer`, maka perintah `"ON"`/`"OFF"` diterapkan ke `buzzerPin` untuk membunyikan atau mematikan buzzer.

#### 3. Penambahan Subscribe pada `hubungkanMQTT()`
```cpp
// Sebelum modifikasi (percobaan 4B):
if (client.connect(clientId.c_str())) {
  client.subscribe(topicPerintah);
  Serial.println("Terhubung dan subscribe topic perintah");
}

// Setelah modifikasi (ditambahkan):
if (client.connect(clientId.c_str())) {
  client.subscribe(topicPerintah);
  client.subscribe(topicBuzzer); // Ditambahkan: Berlangganan topic buzzer
  Serial.println("Terhubung dan subscribe topic perintah & buzzer");
}
```
- Menambahkan baris `client.subscribe(topicBuzzer)` agar ESP8266 juga mendengarkan pesan masuk yang dikirim ke topic buzzer setelah terkoneksi ke broker.

#### 4. Penambahan Konfigurasi Pin pada `setup()`
```cpp
// Sebelum modifikasi (percobaan 4B):
pinMode(ledPin, OUTPUT);
dht.begin();

// Setelah modifikasi (ditambahkan):
pinMode(ledPin, OUTPUT);
pinMode(buzzerPin, OUTPUT); // Ditambahkan: Set pin buzzer sebagai OUTPUT
dht.begin();
```
- Menambahkan baris `pinMode(buzzerPin, OUTPUT)` untuk mengonfigurasi pin GPIO 14 sebagai pin keluaran pengendali buzzer.
