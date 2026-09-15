// Mengimpor library ESP8266WiFi untuk menggunakan fitur WiFi pada modul ESP8266
#include <ESP8266WiFi.h>

// Mendefinisikan SSID (nama jaringan WiFi) yang akan dituju oleh ESP8266 sebagai Station
const char* ssid = "S24";
// Mendefinisikan password dari jaringan WiFi yang akan dimasuki
const char* password = "11111111";

// Mendefinisikan pin GPIO 2 sebagai pin LED indikator status koneksi WiFi
const int ledPin = 2; // LED indikator status koneksi

// Fungsi setup() dijalankan sekali saat ESP8266 pertama kali dinyalakan atau di-reset
void setup() {
  // Memulai komunikasi Serial dengan baud rate 115200 untuk keperluan debug/monitoring
  Serial.begin(115200);
  // Mengatur pin LED sebagai OUTPUT agar dapat dikontrol (nyala/mati)
  pinMode(ledPin, OUTPUT);
  // Mematikan LED saat awal boot (LOW = mati) sebagai kondisi awal
  digitalWrite(ledPin, LOW);

  // Mengatur mode WiFi ESP8266 sebagai Station (STA), yaitu perangkat yang terhubung ke router
  WiFi.mode(WIFI_STA);
  // Memulai proses koneksi ke jaringan WiFi menggunakan SSID dan password yang didefinisikan
  WiFi.begin(ssid, password);

  // Mencetak teks "Menghubungkan ke WiFi" ke Serial Monitor tanpa pindah baris
  Serial.print("Menghubungkan ke WiFi");
  // Loop yang terus berjalan selama status WiFi belum terhubung (WL_CONNECTED)
  while (WiFi.status() != WL_CONNECTED) {
    // Menunggu 500 milidetik (0.5 detik) sebelum cek status berikutnya
    delay(500);
    // Mencetak titik "." sebagai animasi loading proses koneksi
    Serial.print(".");
  }

  // Mencetak baris kosong (newline) setelah titik-titik loading selesai
  Serial.println();
  // Mencetak pesan bahwa WiFi berhasil terhubung
  Serial.println("WiFi berhasil terhubung!");
  // Mencetak label "IP Address : " tanpa pindah baris
  Serial.print("IP Address : ");
  // Mencetak alamat IP lokal yang diberikan oleh router ke ESP8266
  Serial.println(WiFi.localIP());

  // Menyalakan LED (HIGH = nyala) sebagai indikator bahwa koneksi WiFi berhasil
  digitalWrite(ledPin, HIGH); // nyalakan LED sebagai indikator terhubung
}

// Fungsi loop() dijalankan berulang-ulang selama ESP8266 menyala
void loop() {
  // Memeriksa apakah ESP8266 saat ini masih terhubung ke WiFi
  if (WiFi.status() == WL_CONNECTED) {
    // Mencetak status "Terhubung" ke Serial Monitor jika koneksi masih aktif
    Serial.println("Status: Terhubung");
  }
  else {
    // Blok ini dijalankan ketika koneksi WiFi terdeteksi terputus
    // Mencetak pesan bahwa koneksi terputus dan sedang mencoba reconnect
    Serial.println("Status: Terputus! Mencoba menghubungkan ulang (reconnect)...");
    // Mematikan LED karena koneksi WiFi hilang
    digitalWrite(ledPin, LOW); // Matikan LED karena koneksi hilang

    // Langkah 1: Memutus sisa sesi koneksi yang mungkin masih menggantung
    WiFi.disconnect();

    // Langkah 2: Memulai kembali proses koneksi ke WiFi dengan SSID dan password yang sama
    WiFi.begin(ssid, password);

    // Langkah 3: Menunggu proses reconnect dengan batas waktu maksimal (timeout)
    // Mendeklarasikan variabel counter untuk menghitung percobaan tunggu
    int waktuTunggu = 0;
    // Loop menunggu koneksi, maksimal 10 kali percobaan (10 x 500ms = 5 detik)
    while (WiFi.status() != WL_CONNECTED && waktuTunggu < 10) {
      // Menunggu 500 milidetik setiap iterasi
      delay(500);
      // Mencetak titik sebagai animasi loading reconnect
      Serial.print(".");
      // Menambah nilai counter setiap iterasi agar tidak melebihi batas maksimal
      waktuTunggu++;
    }
    // Mencetak newline setelah proses tunggu selesai
    Serial.println();

    // Langkah 4: Mengevaluasi apakah proses reconnect berhasil atau gagal
    if (WiFi.status() == WL_CONNECTED) {
      // Jika berhasil terhubung kembali, cetak pesan sukses
      Serial.println("Berhasil terhubung kembali!");
      // Mencetak label "IP Address : " tanpa pindah baris
      Serial.print("IP Address : ");
      // Mencetak alamat IP baru yang diperoleh setelah reconnect
      Serial.println(WiFi.localIP());
      // Menyalakan LED kembali sebagai tanda koneksi berhasil
      digitalWrite(ledPin, HIGH); // Nyalakan LED kembali
    } else {
      // Jika reconnect gagal setelah timeout, cetak pesan kegagalan
      // ESP8266 akan mencoba lagi pada iterasi loop() berikutnya
      Serial.println("Gagal menghubungkan ulang. Akan mencoba lagi pada siklus berikutnya.");
    }
  }

  // Menunggu 5000 milidetik (5 detik) sebelum melakukan pengecekan status koneksi berikutnya
  delay(5000);
}