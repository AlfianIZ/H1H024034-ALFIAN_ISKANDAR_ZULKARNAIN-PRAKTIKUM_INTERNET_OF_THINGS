# Praktikum Modul 1 — Akuisisi Data Sensor & Kendali Aktuator

## 1. Penjelasan Singkat Percobaan
Pada percobaan praktikum modul 1 ini dilakukan akuisisi data sensor dan kendali aktuator berbasis ESP8266. Tahapan yang dilakukan meliputi pembacaan data sensor suhu dan kelembaban menggunakan DHT11, pemrosesan data di mikrokontroler, dan pengendalian aktuator berupa relay berdasarkan nilai ambang batas (threshold) yang telah ditetapkan.

## 2. Library / Dependencies
Library yang diperlukan :
- **DHT sensor library** (oleh Adafruit) untuk membaca sensor DHT11.



## 3. Penjelasan Kode

### Percobaan 1A

```cpp
// Mengimpor library DHT untuk membaca sensor suhu dan kelembaban
#include <DHT.h>

#define DHTPIN 4      // Mendefinisikan pin data sensor DHT11 terhubung ke GPIO 4
#define DHTTYPE DHT11 // Mendefinisikan tipe sensor yang digunakan adalah DHT11

// Membuat objek dht dengan pin dan tipe sensor yang sudah didefinisikan
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);               // Menginisialisasi komunikasi serial dengan baud rate 115200
  dht.begin();                        // Menginisialisasi sensor DHT11
  Serial.println("Memulai akuisisi data sensor DHT11..."); // Menampilkan pesan awal ke Serial Monitor
}

void loop() {
  float kelembaban = dht.readHumidity();    // Membaca nilai kelembaban dari sensor (dalam %)
  float suhu = dht.readTemperature();       // Membaca nilai suhu dari sensor (dalam °C)

  // Mengecek apakah data yang dibaca valid
  if (isnan(kelembaban) || isnan(suhu)) {
    Serial.println("Gagal membaca data dari sensor DHT11!"); // Menampilkan pesan error jika pembacaan gagal
  } else {
    // Menampilkan nilai suhu dan kelembaban di Serial Monitor
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C, Kelembaban: ");
    Serial.print(kelembaban);
    Serial.println(" %");
  }

  delay(2000);  // Jeda 2 detik antar pembacaan sesuai batasan sensor DHT11
}
```

**Penjelasan kode percobaan 1A:**
- Fungsi `setup()` dijalankan sekali saat startup untuk menginisialisasi komunikasi serial dan sensor DHT11.
- Fungsi `loop()` berjalan terus-menerus untuk membaca data sensor.
- `dht.readHumidity()` dan `dht.readTemperature()` digunakan untuk mengambil nilai kelembaban dan suhu.
- `isnan()` memeriksa apakah hasil pembacaan valid; jika salah satu nilai gagal dibaca, program menampilkan pesan error.
- Jika data valid, nilai suhu dan kelembaban ditampilkan di Serial Monitor.
- `delay(2000)` memberikan jeda minimal 2 detik agar sensor siap untuk pembacaan berikutnya.

### Percobaan 2A

```cpp
// Mengimpor library DHT untuk membaca sensor suhu dan kelembaban
#include <DHT.h>

#define DHTPIN 4       // pin data DHT11 terhubung ke GPIO 4
#define DHTTYPE DHT11  // tipe sensor yang digunakan
#define RELAYPIN D1    // pin kendali relay

// Membuat objek sensor DHT dengan pin dan tipe yang sudah didefinisikan
DHT dht(DHTPIN, DHTTYPE);

const float suhuThreshold = 30.0;  // ambang batas suhu

void setup() {
  Serial.begin(115200);  // inisialisasi komunikasi serial dengan baud rate 115200
  dht.begin();           // inisialisasi sensor DHT11

  pinMode(RELAYPIN, OUTPUT);          // mengatur pin relay sebagai OUTPUT
  digitalWrite(RELAYPIN, LOW);        // aktuator mati di awal

  Serial.println("Memulai akuisisi data sensor DHT11...");  // pesan awal ke Serial Monitor
}

void loop() {
  float suhu = dht.readTemperature();  // membaca nilai suhu dari sensor

  // mengecek apakah data suhu valid (isnan = true jika pembacaan gagal)
  if (isnan(suhu)) {
    Serial.println("Gagal membaca data dari sensor DHT11!");  // tampilkan pesan error
  } else {
    // menampilkan nilai suhu ke Serial Monitor
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");

    // Kendali aktuator berdasarkan hasil akuisisi data sensor
    if (suhu > suhuThreshold) {
      digitalWrite(RELAYPIN, HIGH);  // aktifkan relay/LED jika suhu melebihi threshold
      Serial.println("Aktuator: ON");
    } else {
      digitalWrite(RELAYPIN, LOW);   // matikan relay/LED jika suhu di bawah threshold
      Serial.println("Aktuator: OFF");
    }
  }

  delay(2000);
}
```

**Penjelasan kode percobaan 2A:**
- Pada percobaan 2A, ditambahkan penendali aktuator berbasis relay menggunakan pin `D1`.
- `suhuThreshold = 30.0` menjadi acuan untuk menyalakan atau mematikan aktuator.
- Di `setup()`, pin relay dikonfigurasi sebagai `OUTPUT` dan diatur ke `LOW` agar aktuator dalam keadaan mati saat sistem pertama kali dijalankan.
- Di `loop()`, nilai suhu dibaca lalu dicek validitasnya menggunakan `isnan()`.
- Jika suhu valid, program masuk ke kondisi percabangan `if (suhu > suhuThreshold)`. Jika suhu di atas ambang batas, relay diaktifkan (`HIGH`); jika tidak, relay dimatikan (`LOW`).
- Status aktuator juga ditampilkan di Serial Monitor untuk memudahkan pengamatan.
- `delay(2000)` menjaga interval pembacaan agar tetap sesuai spesifikasi sensor DHT11.

## 4. Jawaban Pertanyaan

### Pertanyaan Percobaan 1A
1. **Gambarkan diagram alur (flowchart) proses akuisisi data sensor DHT22 pada program di atas!**  
   [gambar]

2. **Apa fungsi dari perintah `isnan()` pada program tersebut?**  
   Fungsi perintah `isnan()` adalah untuk memeriksa apakah suatu nilai float bukan merupakan angka yang valid. Jika sensor gagal membaca dan menghasilkan `NaN`, perintah `isnan()` memungkinkan program membedakan antara data valid dan data gagal/error, sehingga program tidak akan menampilkan nilai yang tidak valid.

3. **Jelaskan mengapa diperlukan jeda (delay) minimal sekitar 2 detik antar pembacaan sensor DHT22!**  
   Delay sekitar 2 detik tersebut diperlukan untuk memberikan waktu sensor menyiapkan pengukuran berikutnya. Dengan jeda ini, data suhu dan kelembaban yang dihasilkan lebih akurat sesuai batasan teknis sensor DHT11.

4. **Modifikasi program agar data suhu dan kelembaban dirata-ratakan dari 5 kali pembacaan sebelum ditampilkan, dan berikan penjelasan di setiap baris kode yang ditambahkan!**  
   ```cpp
   // Mengimpor library DHT untuk membaca sensor suhu dan kelembaban
   #include <DHT.h>

   #define DHTPIN 4      // Mendefinisikan pin data sensor DHT11 terhubung ke GPIO 4
   #define DHTTYPE DHT11 // Mendefinisikan tipe sensor yang digunakan adalah DHT11

   #define JUMLAH_SAMPLING 5 // Menentukan jumlah pembacaan yang akan dirata-ratakan (5 kali)

   // Membuat objek dht dengan pin dan tipe sensor yang sudah didefinisikan
   DHT dht(DHTPIN, DHTTYPE);

   void setup() {
     Serial.begin(115200); // Menginisialisasi komunikasi serial dengan baud rate 115200
     dht.begin();           // Menginisialisasi sensor DHT11
     Serial.println("Memulai akuisisi data sensor DHT11 (rata-rata 5 pembacaan)..."); // Menampilkan pesan awal ke Serial Monitor
   }

   void loop() {
     float totalSuhu = 0;       // Variabel untuk menyimpan total akumulasi suhu dari beberapa pembacaan
     float totalKelembaban = 0; // Variabel untuk menyimpan total akumulasi kelembaban dari beberapa pembacaan
     int dataValid = 0;         // Menghitung berapa kali pembacaan yang berhasil (valid, bukan NaN)

     // Perulangan untuk membaca sensor sebanyak JUMLAH_SAMPLING (5) kali
     for (int i = 0; i < JUMLAH_SAMPLING; i++) {
         float kelembaban = dht.readHumidity();    // Membaca nilai kelembaban dari sensor (dalam %)
         float suhu = dht.readTemperature();       // Membaca nilai suhu dari sensor (dalam °C)

         // Mengecek apakah data hasil pembacaan pada iterasi ini valid
         if (!isnan(kelembaban) && !isnan(suhu)) {
             totalSuhu += suhu;             // Menambahkan nilai suhu yang valid ke total akumulasi
             totalKelembaban += kelembaban; // Menambahkan nilai kelembaban yang valid ke total akumulasi
             dataValid++;                   // Menambah penghitung jumlah data yang berhasil dibaca
         } else {
             Serial.println("Pembacaan gagal, dilewati..."); // Menampilkan info jika salah satu pembacaan dalam sampling gagal
         }

         delay(2000); // Memberi jeda 2 detik sebelum melakukan pembacaan berikutnya (sesuai batas minimal DHT11)
     }

     // Mengecek apakah ada minimal satu data valid agar tidak terjadi pembagian dengan nol
     if (dataValid > 0) {
         float rataSuhu = totalSuhu / dataValid;             // Menghitung rata-rata suhu dari data yang valid saja
         float rataKelembaban = totalKelembaban / dataValid; // Menghitung rata-rata kelembaban dari data yang valid saja

         // Menampilkan hasil rata-rata suhu dan kelembaban ke Serial Monitor
         Serial.print("Rata-rata Suhu: ");
         Serial.print(rataSuhu);
         Serial.print(" °C, Rata-rata Kelembaban: ");
         Serial.print(rataKelembaban);
         Serial.println(" %");
     } else {
         Serial.println("Semua pembacaan gagal, tidak ada data untuk dirata-ratakan!"); // Pesan jika seluruh 5 pembacaan gagal
     }
   }
   ```

   **Penjelasan kode:**
   - Kode ini menambahkan mekanisme pembacaan berulang sebanyak 5 kali menggunakan `for` loop sebelum menampilkan hasil.
   - Variabel `totalSuhu` dan `totalKelembaban` digunakan untuk mengakumulasi nilai pembacaan yang valid.
   - `dataValid` berfungsi menghitung berapa kali pembacaan berhasil, agar rata-rata tidak dibagi oleh nilai yang gagal.
   - Kondisi `if (!isnan(kelembaban) && !isnan(suhu))` memastikan hanya data valid yang masuk ke perhitungan.
   - Setelah loop, program mengecek `dataValid > 0` untuk menghindari pembagian dengan nol jika seluruh pembacaan gagal.
   - `delay(2000)` tetap diberikan di dalam loop karena sensor DHT11 memerlukan jeda minimal 2 detik antar pembacaan.

### Pertanyaan Percobaan 2A
1. **Mengapa diperlukan nilai ambang batas (threshold) dalam sistem kendali aktuator berbasis sensor?**  
   Nilai ambang batas diperlukan untuk menentukan kapan aktuator harus melakukan aksi fisik (menyala/mematikan). Threshold menjadi referensi bagi mikrokontroler untuk mengambil keputusan berdasarkan data sensor.

2. **Jelaskan apa yang akan terjadi apabila nilai `suhuThreshold` diturunkan menjadi sangat rendah, misalnya 20.0!**  
   Jika `suhuThreshold` diturunkan menjadi 20.0, aktuator akan menyala lebih sering karena syarat suhu > 20.0 lebih mudah terpenuhi. Pada suhu ruang sekitar 21–25 °C, relay akan terus-menerus dalam keadaan ON.

3. **Apa perbedaan antara kendali aktuator secara terus-menerus (kondisi tunggal) dengan kendali menggunakan histeresis (dua ambang batas)?**  
   Kendali kondisi tunggal hanya menggunakan satu ambang batas untuk menentukan ON dan OFF, sehingga fluktuasi kecil di sekitar nilai ambang dapat menyebabkan aktuator menyalar dan mati berulang kali dalam waktu singkat. Histeresis menggunakan dua ambang batas (atas dan bawah) sehingga terbentuk zona mati; aktuator tidak akan mengganti status kecuali suhu melewati salah satu ambang, mencegah chattering dan memperpanjang umur relay.

4. **Modifikasi program agar menggunakan dua ambang batas (histeresis), misalnya aktuator menyala pada suhu di atas 30°C dan baru mati pada suhu di bawah 28°C, dan berikan penjelasan di setiap baris kodenya**  
   ```cpp
   // Mengimpor library DHT untuk membaca sensor suhu dan kelembaban
   #include <DHT.h>

   #define DHTPIN 4       // pin data DHT11 terhubung ke GPIO 4
   #define DHTTYPE DHT11  // tipe sensor yang digunakan
   #define RELAYPIN D1    // pin kendali relay

   // Membuat objek sensor DHT dengan pin dan tipe yang sudah didefinisikan
   DHT dht(DHTPIN, DHTTYPE);

   // ==== Konfigurasi Histeresis ====
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
   ```

   **Penjelasan kode:**
   - Kode ini mengubah kendali aktuator dari kondisi tunggal menjadi kendali dengan dua ambang batas atau histerisis.
   - `upperThreshold` dan `lowerThreshold` menentukan titik menyala dan mati, sehingga membentuk zona mati di antaranya.
   - `aktuatorStatus` disimpan di luar `loop()` agar nilainya tetap terjaga antar siklus loop dan tidak ter-reset.
   - Jika suhu melewati `upperThreshold`, aktuator diset ON; jika turun di bawah `lowerThreshold`, aktuator diset OFF.
   - Jika suhu berada di antara kedua ambang, status aktuator tidak berubah, mencegah aktifasi berulang akibat fluktuasi kecil.
   - `digitalWrite(RELAYPIN, aktuatorStatus ? HIGH : LOW)` memanfaatkan operator ternary untuk menerapkan status secara singkat.

### Pertanyaan Analisis
1. **Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan!**  
   Pada percobaan 1A program berhasil melakukan akuisisi data berupa suhu dan kelembaban dari sensor DHT11 dan menampilkannya pada serial monitor. Pada percobaan 2A program berhasil mengendalikan aktuator berupa relay berdasarkan nilai threshold, dimana jika suhu di atas atau sama dengan 30 derajat maka relay akan ON, jika di bawah threshold relay OFF. Percobaan ini berhasil mengendalikan relay ketika suhu 31 derajat relay ON dan pada suhu 27 derajat relay OFF.

2. **Bagaimana pengaruh akurasi dan waktu tanggap (response time) sensor terhadap kecepatan reaksi aktuator pada sistem IoT?**  
   Akurasi sensor menentukan seberapa tepat data yang menjadi dasar keputusan aktuator; sensor dengan akurasi rendah bisa membuat aktuator salah bereaksi. Response time memengaruhi seberapa cepat sensor membaca perubahan keadaan di dunia nyata; semakin besar jedanya maka aktuator akan terlambat merespons perubahan kondisi.

3. **Bagaimana cara kerja sistem dalam mengubah data sensor menjadi keputusan kendali aktuator (proses akuisisi hingga aktuasi)?**  
   Pertama, sensor akan mendeteksi suhu. Kedua, sensor mengirimkan data suhu ke mikrokontroler. Ketiga, mikrokontroler memproses data tersebut apakah suhu melebihi threshold atau tidak. Keempat, jika suhu melebihi atau sama dengan threshold maka mikrokontroler memerintahkan relay untuk ON, jika suhu di bawah threshold maka relay OFF.

4. **Bagaimana kombinasi antara akuisisi data sensor dan kendali aktuator dapat digunakan untuk membangun sistem IoT yang responsif terhadap perubahan kondisi lingkungan, misalnya pada sistem smart farming atau smart home?**  
   Pada sistem smart farming, kombinasi ini dapat digunakan untuk penyiraman otomatis berdasarkan kelembaban tanah. Sensor soil moisture akan mengumpulkan data kelembaban tanah lalu dikirim ke mikrokontroler untuk memutuskan apakah melebihi threshold atau tidak, yang kemudian akan menyalakan pompa air atau tidak menyalakannya berdasarkan hasil keputusan tersebut.
