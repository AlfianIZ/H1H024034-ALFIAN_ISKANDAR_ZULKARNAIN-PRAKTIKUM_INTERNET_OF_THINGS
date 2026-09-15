// Mengimpor library DHT untuk membaca sensor suhu dan kelembaban
#include <DHT.h>

#define DHTPIN 4       // pin data DHT11 terhubung ke GPIO 4
#define DHTTYPE DHT11  // tipe sensor yang digunakan
#define RELAYPIN D1    // pin kendali relay

// Membuat objek sensor DHT dengan pin dan tipe yang sudah didefinisikan
DHT dht(DHTPIN, DHTTYPE);

// ==== Konfigurasi Histerisis ====
const float upperThreshold = 30.0;  // ambang batas ATAS -> aktuator menyala jika suhu melebihi ini
const float lowerThreshold = 28.0;  // ambang batas BAWAH -> aktuator mati jika suhu turun di bawah ini
// Selisih antara upperThreshold dan lowerThreshold disebut "zona mati" (dead band),
// yaitu rentang suhu di mana status aktuator TIDAK berubah walaupun suhu naik/turun sedikit

bool aktuatorStatus = false;  // variabel penyimpan status aktuator (true = ON, false = OFF)
                               // dideklarasikan di luar loop() agar nilainya tidak reset setiap kali loop() dijalankan

void setup() {
  Serial.begin(115200);  // inisialisasi komunikasi serial dengan baud rate 115200
  dht.begin();           // inisialisasi sensor DHT11

  pinMode(RELAYPIN, OUTPUT);          // mengatur pin relay sebagai OUTPUT
  digitalWrite(RELAYPIN, LOW);        // aktuator mati di awal

  Serial.println("Memulai akuisisi data sensor DHT11 dengan kendali histerisis...");  // pesan awal ke Serial Monitor
}

void loop() {
  float suhu = dht.readTemperature();  // membaca nilai suhu dari sensor

  // mengecek apakah data suhu valid (isnan = true jika pembacaan gagal)
  if (isnan(suhu)) {
    Serial.println("Gagal membaca data dari sensor DHT11!");  // tampilkan pesan error jika pembacaan gagal
  } else {
    // menampilkan nilai suhu ke Serial Monitor
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");

    // ==== Logika Kendali Histerisis ====
    if (suhu > upperThreshold) {
      // jika suhu melewati ambang atas, aktuator diset ON
      aktuatorStatus = true;
    } else if (suhu < lowerThreshold) {
      // jika suhu turun di bawah ambang bawah, aktuator diset OFF
      aktuatorStatus = false;
    }
    // catatan: jika suhu berada DI ANTARA lowerThreshold dan upperThreshold (28.0 - 30.0),
    // maka tidak ada perintah yang dijalankan di sini,
    // sehingga aktuatorStatus TETAP mempertahankan nilai sebelumnya (tidak berubah)

    // menerapkan status aktuator ke pin relay sesuai hasil logika histerisis
    digitalWrite(RELAYPIN, aktuatorStatus ? HIGH : LOW);
    // operator ternary: jika aktuatorStatus true -> kirim HIGH (nyala), jika false -> kirim LOW (mati)

    // menampilkan status aktuator saat ini ke Serial Monitor
    Serial.println(aktuatorStatus ? "Aktuator: ON" : "Aktuator: OFF");
  }

  delay(2000); 
}