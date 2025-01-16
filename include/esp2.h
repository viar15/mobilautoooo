#include <esp_now.h>
#include <WiFi.h>

const int lightME = 15;

// Struktur data untuk dikirim
struct __attribute__((packed)) dataPacket {
  char message[50];
};

// Callback ketika data diterima
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  // Dekode data
  dataPacket packet;
  memcpy(&packet, incomingData, sizeof(packet));
  // Tampilkan pesan yang diterima
  Serial.print("Received Message: ");
  Serial.println(packet.message);
}

void setup() {
  // Mulai komunikasi serial
  Serial.begin(115200);
  Serial.println("ESP32 Receiver");

  // Set ESP32 sebagai Station
  WiFi.mode(WIFI_STA);

  // Inisialisasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Daftarkan callback untuk menerima data
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Tidak ada kode di loop, semua diproses melalui callback
}
