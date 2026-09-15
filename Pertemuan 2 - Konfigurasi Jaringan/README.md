# Praktikum Modul 2 — Konfigurasi Jaringan

## 1. Penjelasan Singkat Percobaan
Percobaan ini bertujuan untuk memahami cara kerja ESP8266 dalam konfigurasi jaringan WiFi, baik sebagai Station yang terhubung ke router maupun sebagai Access Point.

## 2. Library / Dependencies
Library yang diperlukan :
- Library ESP8266WiFi.h


## 3. Penjelasan Kode

### Percobaan 2A

| Baris Kode | Penjelasan |
|---|---|
| `#include <ESP8266WiFi.h>` | Mengimpor library ESP8266WiFi agar fungsi-fungsi WiFi pada modul ESP8266 dapat digunakan. |
| `const char* ssid = "wle";` | Mendeklarasikan variabel bertipe string konstanta untuk menyimpan nama jaringan WiFi (SSID) yang akan dituju. |
| `const char* password = "pppppppp";` | Mendeklarasikan variabel bertipe string konstanta untuk menyimpan kata sandi jaringan WiFi tersebut. |
| `#define LED_PIN 2` | Mendefinisikan konstanta `LED_PIN` dengan nilai `2`, yang merupakan nomor pin GPIO tempat LED terhubung (GPIO2 / D4 pada NodeMCU). |
| `Serial.begin(115200);` | Menginisialisasi komunikasi Serial dengan baud rate 115200 bps agar data dapat dikirim ke Serial Monitor. |
| `pinMode(LED_PIN, OUTPUT);` | Mengatur pin LED_PIN (GPIO2) sebagai pin keluaran (OUTPUT) sehingga bisa dikendalikan untuk menyalakan atau mematikan LED. |
| `digitalWrite(LED_PIN, LOW);` | Mematikan LED saat awal program berjalan (kondisi awal: LED mati). |
| `WiFi.mode(WIFI_STA);` | Mengatur ESP8266 agar bekerja dalam mode **Station (STA)**, yaitu sebagai klien yang terhubung ke jaringan WiFi yang sudah ada. |
| `WiFi.begin(ssid, password);` | Memulai proses koneksi WiFi menggunakan SSID dan password yang telah didefinisikan sebelumnya. |
| `Serial.println();` | Mencetak baris kosong di Serial Monitor untuk memberi jarak visual. |
| `Serial.print("Menghubungkan ke WiFi");` | Menampilkan teks "Menghubungkan ke WiFi" di Serial Monitor tanpa pindah baris. |
| `while (WiFi.status() != WL_CONNECTED) {` | Membuat perulangan yang terus berjalan **selama** ESP8266 belum berhasil terhubung ke WiFi. `WL_CONNECTED` adalah konstanta yang menandakan koneksi berhasil. |
| `delay(500);` | Memberikan jeda 500 milidetik (0,5 detik) di setiap iterasi agar tidak membebani prosesor dan menunggu koneksi terbentuk. |
| `Serial.print(".");` | Mencetak karakter titik (`.`) setiap 500ms sebagai indikator visual bahwa proses koneksi sedang berlangsung. |
| `Serial.println("WiFi berhasil terhubung!");` | Setelah while loop selesai (koneksi berhasil), mencetak pesan konfirmasi ke Serial Monitor. |
| `digitalWrite(LED_PIN, HIGH);` | Menyalakan LED sebagai indikator fisik bahwa ESP8266 telah berhasil terhubung ke jaringan WiFi. |
| `Serial.print("SSID        : ");` `Serial.println(WiFi.SSID());` | Menampilkan nama SSID jaringan WiFi yang sedang terhubung ke Serial Monitor. |
| `Serial.print("IP Address  : ");` `Serial.println(WiFi.localIP());` | Menampilkan alamat IP lokal yang diberikan oleh router ke ESP8266 (diperoleh dari DHCP). |
| `Serial.print("MAC Address : ");` `Serial.println(WiFi.macAddress());` | Menampilkan alamat MAC (Media Access Control) unik milik modul ESP8266. |
| `Serial.print("RSSI        : ");` `Serial.print(WiFi.RSSI());` `Serial.println(" dBm");` | Menampilkan kekuatan sinyal WiFi dalam satuan dBm (Received Signal Strength Indicator). Nilai lebih mendekati 0 berarti sinyal lebih kuat. |
| `if (WiFi.status() == WL_CONNECTED) {` | Pada fungsi `loop()`, memeriksa secara periodik apakah ESP8266 masih dalam kondisi terhubung ke WiFi. |
| `Serial.println("Status: Terhubung");` | Jika masih terhubung, mencetak pesan "Status: Terhubung" ke Serial Monitor. |
| `Serial.println("Status: Terputus");` | Jika koneksi terputus, mencetak pesan "Status: Terputus". |
| `digitalWrite(LED_PIN, LOW);` | Jika koneksi terputus, LED dimatikan sebagai indikator visual bahwa jaringan tidak lagi tersambung. |
| `delay(5000);` | Memberikan jeda 5 detik sebelum pengecekan status berikutnya pada loop. |


### Percobaan 2B

**Penjelasan kode percobaan 2B:**

| Baris Kode | Penjelasan |
|---|---|
| `#include <ESP8266WiFi.h>` | Mengimpor library ESP8266WiFi agar fungsi-fungsi WiFi pada modul ESP8266 dapat digunakan, termasuk fitur Access Point. |
| `const char* ap_ssid = "ESP8266 CIIDUK WARRIOR";` | Mendeklarasikan variabel string konstanta untuk menyimpan nama SSID (nama hotspot) yang akan disiarkan oleh ESP8266 sebagai Access Point. |
| `const char* ap_password = "12345678";` | Mendeklarasikan variabel string konstanta untuk menyimpan kata sandi Access Point. Minimal 8 karakter untuk jaringan WPA2. |
| `Serial.begin(115200);` | Menginisialisasi komunikasi Serial dengan baud rate 115200 bps agar output dapat dipantau melalui Serial Monitor. |
| `WiFi.mode(WIFI_AP);` | Mengatur ESP8266 agar bekerja dalam mode **Access Point (AP)**, yaitu membuat jaringan WiFi sendiri yang dapat diakses perangkat lain (bukan terhubung ke router). |
| `WiFi.softAP(ap_ssid, ap_password);` | Mengaktifkan Access Point dengan SSID dan password yang telah ditentukan. Setelah baris ini, ESP8266 mulai menyiarkan sinyal WiFi. |
| `IPAddress apIP = WiFi.softAPIP();` | Mengambil alamat IP dari Access Point yang baru dibuat dan menyimpannya ke variabel `apIP`. Secara default, IP Access Point ESP8266 adalah `192.168.4.1`. |
| `Serial.println("Access Point aktif!");` | Mencetak pesan konfirmasi ke Serial Monitor bahwa Access Point telah berhasil diaktifkan. |
| `Serial.print("SSID : ");` `Serial.println(ap_ssid);` | Menampilkan nama SSID Access Point yang sedang disiarkan ke Serial Monitor. |
| `Serial.print("IP Address: ");` `Serial.println(apIP);` | Menampilkan alamat IP Access Point ke Serial Monitor. Perangkat lain yang terhubung akan menggunakan IP ini sebagai gateway. |
| `int jumlahClient = WiFi.softAPgetStationNum();` | Pada fungsi `loop()`, mengambil jumlah perangkat (station/klien) yang sedang aktif terhubung ke Access Point ESP8266, disimpan ke variabel `jumlahClient`. |
| `Serial.print("Jumlah perangkat terhubung: ");` `Serial.println(jumlahClient);` | Menampilkan jumlah perangkat yang saat ini terhubung ke Access Point ke Serial Monitor. |
| `delay(5000);` | Memberikan jeda 5 detik sebelum pengecekan jumlah klien berikutnya pada loop. |



### Kode Modifikasi

| Baris Kode | Penjelasan |
|---|---|
| `const int ledPin = 2;` | Mendeklarasikan pin LED menggunakan `int` (berbeda dari versi asli yang menggunakan `#define`), nilainya tetap GPIO2 / D4. |
| `digitalWrite(ledPin, HIGH);` *(di setup)* | Menyalakan LED di akhir `setup()` sebagai tanda bahwa koneksi WiFi awal berhasil. |
| `Serial.println("Status: Terputus! Mencoba menghubungkan ulang (reconnect)...");` | Menampilkan pesan peringatan ke Serial Monitor bahwa koneksi terputus dan proses reconnect akan segera dimulai. |
| `digitalWrite(ledPin, LOW);` *(di else)* | Mematikan LED saat koneksi terputus sebagai indikator visual bahwa perangkat sedang tidak terhubung. |
| `WiFi.disconnect();` | Memutus koneksi WiFi yang sedang gagal secara eksplisit sebelum mencoba ulang, agar proses reconnect dimulai dari keadaan bersih. |
| `WiFi.begin(ssid, password);` *(di else)* | Memulai ulang proses koneksi WiFi dengan SSID dan password yang sama seperti semula. |
| `int waktuTunggu = 0;` | Mendeklarasikan variabel counter `waktuTunggu` sebagai pembatas waktu tunggu reconnect agar tidak menunggu selamanya. |
| `while (WiFi.status() != WL_CONNECTED && waktuTunggu < 10) {` | Perulangan yang menunggu koneksi berhasil, namun dibatasi maksimal **10 kali iterasi** (setara ±5 detik). Kondisi `waktuTunggu < 10` mencegah infinite loop jika WiFi tidak bisa terhubung. |
| `waktuTunggu++;` | Menambah nilai counter setiap iterasi agar batas waktu tunggu dapat terpantau dan loop bisa berhenti tepat waktu. |
| `Serial.println();` | Mencetak baris baru setelah selesai mencetak titik-titik indikator proses reconnect. |
| `if (WiFi.status() == WL_CONNECTED) {` *(di dalam else)* | Memeriksa apakah proses reconnect berhasil setelah while loop selesai. |
| `Serial.println("Berhasil terhubung kembali!");` | Mencetak pesan sukses jika ESP8266 berhasil terhubung kembali ke jaringan WiFi. |
| `Serial.println(WiFi.localIP());` *(reconnect)* | Menampilkan IP Address baru yang diberikan router setelah berhasil reconnect (IP bisa berbeda dari sebelumnya). |
| `digitalWrite(ledPin, HIGH);` *(setelah reconnect berhasil)* | Menyalakan kembali LED sebagai tanda bahwa koneksi WiFi telah pulih. |
| `Serial.println("Gagal menghubungkan ulang. Akan mencoba lagi pada siklus berikutnya.");` | Jika setelah 10 iterasi masih belum terhubung, mencetak pesan gagal. Proses akan dicoba lagi di iterasi `loop()` berikutnya (5 detik kemudian). |
| `delay(5000);` *(di loop)* | Memberikan jeda 5 detik sebelum pengecekan status berikutnya, baik saat terhubung maupun setelah reconnect gagal. |


## 4. Hasil Pengamatan
### Pengamatan Koneksi WiFi Mode Station (STA)
| No. | Waktu (s) | Status Koneksi | IP Address | MAC Address | RSSI (dBm) | Status LED |
|---|---|---|---|---|---|---|
| 1 | 0 | terhubung | 10.188.91.76 | B4:E6:2D:3A:8F:BC | -42 | nyala |
| 2 | 5 | terhubung | 10.188.91.76 | B4:E6:2D:3A:8F:BC | -42 | nyala |
| 3 | 10 | terhubung | 10.188.91.76 | B4:E6:2D:3A:8F:BC | -42 | nyala |
| 4 | 15 | terhubung | 10.188.91.76 | B4:E6:2D:3A:8F:BC | -42 | nyala |
| 5 | 20 | terhubung | 10.188.91.76 | B4:E6:2D:3A:8F:BC | -42 | nyala |
| 6 | 25 | terhubung | 10.206.11.76 | B4:E6:2D:3A:8F:BC | -39 | nyala |
| 7 | 30 | terhubung | 10.206.11.76 | B4:E6:2D:3A:8F:BC | -39 | nyala |
| 8 | 35 | terhubung | 10.206.11.76 | B4:E6:2D:3A:8F:BC | -39 | nyala |
| 9 | 40 | terhubung | 10.206.11.76 | B4:E6:2D:3A:8F:BC | -39 | nyala |
| 10 | 45 | terhubung | 10.206.11.76 | B4:E6:2D:3A:8F:BC | -39 | nyala |

### Pengujian Mode Station (STA)
| No. | Kondisi Pengujian | SSID | Password | Status Koneksi | Output Serial Monitor | Keterangan |
|---|---|---|---|---|---|---|
| 1 | Kredensial Benar | wle | pppppppp | terhubung | Status: terhubung | Led menyala |
| 2 | Password Salah | wle | 11111111 | Tidak terhubung | ………………… (hanya titik titik) | Led mati karena password salah |
| 3 | SSID Salah | S24 | pppppppp | Tidak terhubung | ………………… (hanya titik titik) | Led mati karena SSID salah |

### Pengamatan Mode Access Point (AP)
| No. | Parameter | Nilai Konfigurasi | Hasil Pengamatan |
|---|---|---|---|
| 1 | SSID | ESP32_AccessPoint | ESP32_AccessPoint |
| 2 | Password | 12345678 | Behasil digunakan |
| 3 | IP Address AP | 192.168.4.1 | 192.168.4.1 |
| 4 | Status Access Point | Aktif | Aktif |
| 5 | SSID terdeteksi pada client | Ya | Ya |
| 6 | Perangkat berhasil terhubung | - | Ya, 1 smartphone |

### Pengamatan Jumlah Perangkat Terhubung ke AP
| No. | Waktu (s) | Jumlah Client | Perangkat yang terhubung | Keterangan |
|---|---|---|---|---|
| 1 | 0 | 1 | Smartphone 1 | Jumlah perangkat terhubung: 1 |
| 2 | 5 | 1 | Smartphone 1 | Jumlah perangkat terhubung: 1 |
| 3 | 10 | 1 | Smartphone 1 | Jumlah perangkat terhubung: 1 |
| 4 | 15 | 2 | Smartphone 1, Smartphone 2 | Jumlah perangkat terhubung: 2 |
| 5 | 20 | 2 | Smartphone 1, Smartphone 2 | Jumlah perangkat terhubung: 2 |
| 6 | 25 | 2 | Smartphone 1, Smartphone 2 | Jumlah perangkat terhubung: 2 |
| 7 | 30 | 3 | Smartphone 1, Smartphone 2, Laptop 1 | Jumlah perangkat terhubung: 3 |
| 8 | 35 | 3 | Smartphone 1, Smartphone 2, Laptop 1 | Jumlah perangkat terhubung: 3 |
| 9 | 40 | 3 | Smartphone 1, Smartphone 2, Laptop 1 | Jumlah perangkat terhubung: 3 |
| 10 | 45 | 3 | Smartphone 1, Smartphone 2, Laptop 1 | Jumlah perangkat terhubung: 3 |