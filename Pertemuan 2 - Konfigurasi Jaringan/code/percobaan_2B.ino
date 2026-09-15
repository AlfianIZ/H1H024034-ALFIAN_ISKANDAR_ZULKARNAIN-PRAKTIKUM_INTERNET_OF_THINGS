// Mengimpor library ESP8266WiFi untuk menggunakan fitur WiFi pada modul ESP8266
#include <ESP8266WiFi.h>

// Mendefinisikan nama SSID (nama jaringan WiFi) yang akan dipancarkan oleh Access Point
const char* ap_ssid = "ESP8266 CIIDUK WARRIOR";
// Mendefinisikan password Access Point (minimal 8 karakter agar valid)
const char* ap_password = "12345678"; // minimal 8 karakter

// Fungsi setup() dijalankan sekali saat ESP8266 pertama kali dinyalakan atau di-reset
void setup() {
  // Memulai komunikasi Serial dengan baud rate 115200 untuk keperluan debug/monitoring
  Serial.begin(115200);

  // Mengatur mode WiFi ESP8266 menjadi Access Point (AP) saja, bukan Station (STA)
  WiFi.mode(WIFI_AP);
  // Menginisialisasi Access Point dengan SSID dan password yang sudah didefinisikan
  WiFi.softAP(ap_ssid, ap_password);

  // Membaca alamat IP dari Access Point yang baru dibuat (default: 192.168.4.1)
  IPAddress apIP = WiFi.softAPIP();

  // Mencetak informasi ke Serial Monitor bahwa Access Point sudah aktif
  Serial.println("Access Point aktif!");
  // Mencetak label "SSID : " ke Serial Monitor tanpa pindah baris
  Serial.print("SSID : ");
  // Mencetak nama SSID Access Point ke Serial Monitor dan pindah baris
  Serial.println(ap_ssid);
  // Mencetak label "IP Address: " ke Serial Monitor tanpa pindah baris
  Serial.print("IP Address: ");
  // Mencetak alamat IP Access Point ke Serial Monitor dan pindah baris
  Serial.println(apIP);
}

// Fungsi loop() dijalankan berulang-ulang selama ESP8266 menyala
void loop() {
  // Membaca jumlah perangkat (client/station) yang sedang terhubung ke Access Point
  int jumlahClient = WiFi.softAPgetStationNum();

  // Mencetak label "Jumlah perangkat terhubung: " ke Serial Monitor tanpa pindah baris
  Serial.print("Jumlah perangkat terhubung: ");
  // Mencetak nilai jumlah client yang terhubung ke Serial Monitor dan pindah baris
  Serial.println(jumlahClient);
  // Menunggu selama 5000 milidetik (5 detik) sebelum loop berikutnya dijalankan
  delay(5000);
}