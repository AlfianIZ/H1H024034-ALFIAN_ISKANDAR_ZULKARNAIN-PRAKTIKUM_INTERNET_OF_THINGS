// include library wifi esp8266
#include <ESP8266WiFi.h>  

// ===============================
// Konfigurasi WiFi
// ===============================
const char* ssid = "wle";
const char* password = "pppppppp";

// LED indikator
// GPIO 2 pada ESP8266 = D4 pada NodeMCU
#define LED_PIN 2

void setup() {
  // Memulai komunikasi Serial
  Serial.begin(115200);

  // Mengatur pin LED sebagai output
  pinMode(LED_PIN, OUTPUT);

  // LED mati saat awal
  digitalWrite(LED_PIN, LOW);

  // Mengatur ESP8266 sebagai Station
  WiFi.mode(WIFI_STA);

  // Memulai koneksi WiFi
  WiFi.begin(ssid, password);

  // mencetak pesan Menghubungkan ke WiFi ketika mikrokontroler mencoba menghubungkan ke wifi
  Serial.println();
  Serial.print("Menghubungkan ke WiFi");

  // Menunggu sampai ESP8266 terhubung
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // mencetak pesan wifi berhasil terhubung ketika mikrokontroler berhasil terhubung ke wifi
  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  // menyalakan LED indikator ketika mikrokontroler berhasil terhubung ke wifi
  digitalWrite(LED_PIN, HIGH);

  // mencetak informasi jaringan ketika mikrokontroler berhasil terhubung ke wifi
  Serial.print("SSID        : ");
  Serial.println(WiFi.SSID());

  Serial.print("IP Address  : ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());

  Serial.print("RSSI        : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

void loop() {
  // Cek status koneksi setiap 5 detik
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status: Terhubung");
  } else {
    Serial.println("Status: Terputus");
    digitalWrite(LED_PIN, LOW);
  }
  delay(5000);
}