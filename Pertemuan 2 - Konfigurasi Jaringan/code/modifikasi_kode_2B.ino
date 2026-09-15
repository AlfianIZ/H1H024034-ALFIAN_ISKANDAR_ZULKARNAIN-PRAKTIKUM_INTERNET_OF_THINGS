#include <ESP8266WiFi.h>

const char* ap_ssid = "ESP8266 CIIDUK WARRIOR";
const char* ap_password = "12345678";

const char* sta_ssid = "NAMA_WIFI_RUMAH";
const char* sta_password = "PASSWORD_WIFI_RUMAH";

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_AP_STA);

  WiFi.softAP(ap_ssid, ap_password);
  IPAddress apIP = WiFi.softAPIP();

  Serial.println("Access Point aktif!");
  Serial.print("AP SSID : ");
  Serial.println(ap_ssid);
  Serial.print("AP IP Address: ");
  Serial.println(apIP);

  Serial.print("Menghubungkan ke WiFi: ");
  Serial.println(sta_ssid);

  WiFi.begin(sta_ssid, sta_password);

  int timeout = 20; 
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }

  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Berhasil terhubung ke WiFi rumah!");
    Serial.print("STA IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Gagal terhubung ke WiFi rumah. Cek SSID/password.");
  }
}

void loop() {
  int jumlahClient = WiFi.softAPgetStationNum();

  Serial.print("Jumlah perangkat terhubung ke AP: ");
  Serial.println(jumlahClient);

  Serial.print("Status koneksi ke WiFi rumah: ");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Terhubung (" + WiFi.localIP().toString() + ")");
  } else {
    Serial.println("Terputus, mencoba menghubungkan ulang...");
    WiFi.reconnect();
  }

  delay(5000);
}