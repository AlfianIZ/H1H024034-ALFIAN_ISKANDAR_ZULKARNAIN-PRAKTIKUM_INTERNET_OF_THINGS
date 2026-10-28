#include <math.h>

const int N = 5;          // Jumlah sampel untuk moving average buffer
float buffer[N];          // Buffer array melingkar untuk menyimpan N pembacaan terakhir
int indeks = 0;           // Posisi penulisan berikutnya pada buffer
int jumlah = 0;           // Jumlah sampel yang telah terisi (maksimal N)

float suhuMin = 1000.0;
float suhuMax = -1000.0;

// Fungsi untuk menghitung moving average data suhu
float hitungMovingAverage(float nilaiBaru) {
  buffer[indeks] = nilaiBaru;
  indeks = (indeks + 1) % N;
  
  if (jumlah < N) {
    jumlah++;
  }

  float total = 0;
  for (int i = 0; i < jumlah; i++) {
    total += buffer[i];
  }
  
  return total / jumlah;
}

// Fungsi untuk menghitung simpangan baku (standard deviation) dari data pada buffer
float hitungSimpanganBaku() {
  // Minimal membutuhkan 2 data untuk menghitung simpangan baku sampel
  if (jumlah < 2) return 0.0;

  // 1. Hitung nilai rata-rata (mean) dari elemen yang ada di buffer
  float total = 0;
  for (int i = 0; i < jumlah; i++) {
    total += buffer[i];
  }
  float rerata = total / jumlah;

  // 2. Hitung jumlah kuadrat selisih dari nilai rata-rata
  float sumKuadrat = 0;
  for (int i = 0; i < jumlah; i++) {
    float selisih = buffer[i] - rerata;
    sumKuadrat += selisih * selisih;
  }

  // 3. Menghitung simpangan baku sampel: s = sqrt( sum((x_i - rerata)^2) / (jumlah - 1) )
  // Catatan: Jika ingin menggunakan simpangan baku populasi, bagi dengan 'jumlah'
  return sqrt(sumKuadrat / (jumlah - 1));
}

// Fungsi mock / simulasi sensor suhu jika tidak menggunakan DHT fisik
float bacaSuhuMock() {
  static unsigned long jumlahBaca = 0;
  jumlahBaca++;

  // Suhu dasar berubah bertahap mengikuti gelombang sinus
  float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);

  // Noise kecil untuk simulasi fluktuasi alami sensor (+-0.5 C)
  float noise = random(-5, 6) * 0.1;

  float suhu = suhuDasar + noise;

  // Simulasi lonjakan anomali suhu setiap 25 kali pembacaan
  if (jumlahBaca % 25 == 0) {
    suhu += 8.0;
  }

  return suhu;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== Memulai Percobaan 5A Modifikasi (Moving Average & Simpangan Baku) ===");
}

void loop() {
  // Mendapatkan nilai suhu (dari simulasi mock atau sensor fisik)
  float suhu = bacaSuhuMock();

  if (isnan(suhu)) {
    Serial.println("Pembacaan gagal, data diabaikan");
  } else {
    // Hitung moving average
    float suhuFilter = hitungMovingAverage(suhu);

    // Update nilai minimum dan maksimum
    if (suhu < suhuMin) suhuMin = suhu;
    if (suhu > suhuMax) suhuMax = suhu;

    // Hitung simpangan baku dari data buffer saat ini
    float simpanganBaku = hitungSimpanganBaku();

    // Tampilkan hasil pengolahan ke Serial Monitor
    Serial.print("Mentah: ");
    Serial.print(suhu, 2);
    Serial.print(" | Filter: ");
    Serial.print(suhuFilter, 2);
    Serial.print(" | Min: ");
    Serial.print(suhuMin, 2);
    Serial.print(" | Max: ");
    Serial.print(suhuMax, 2);
    Serial.print(" | StdDev: ");
    Serial.println(simpanganBaku, 4);
  }

  delay(2000);
}

