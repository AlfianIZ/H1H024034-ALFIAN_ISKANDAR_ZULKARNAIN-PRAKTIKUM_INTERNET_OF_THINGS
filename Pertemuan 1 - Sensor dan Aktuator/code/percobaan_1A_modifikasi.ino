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