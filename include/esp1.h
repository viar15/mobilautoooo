// #include <esp_now.h>
// #include <WiFi.h>

// // Alamat MAC tujuan
// uint8_t broadcastAddress[] = {0x10, 0x06, 0x1C, 0x86, 0xB9, 0xBC};

// // Struktur data untuk dikirim
// struct __attribute__((packed)) dataPacket {
//   int state;
// };

// esp_now_peer_info_t peerInfo;

// // Callback saat data dikirim
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("\r\nLast Packet Send Status:\t");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }

// void setup() {
//   Serial.begin(115200);

//   // Set perangkat sebagai WiFi Station
//   WiFi.mode(WIFI_STA);

//   // Inisialisasi ESP-NOW
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }

//   // Registrasi callback untuk status pengiriman
//   esp_now_register_send_cb(OnDataSent);

//   // Set detail peer
//   memcpy(peerInfo.peer_addr, broadcastAddress, 6);
//   peerInfo.channel = 0; // Pastikan channel sesuai dengan penerima
//   peerInfo.encrypt = false;

//   // Tambahkan peer
//   if (esp_now_add_peer(&peerInfo) != ESP_OK) {
//     Serial.println("Failed to add peer");
//     return;
//   }
// }

// void loop() {
//   dataPacket packet;

//   // Pastikan pin yang akan dibaca telah diatur sebelumnya
//   int pushdownPin = 26; // Ubah sesuai pin push button Anda
//   pinMode(pushdownPin, INPUT);

//   // Baca nilai dari pin push button
//   packet.state = digitalRead(pushdownPin);

//   // Kirim data menggunakan ESP-NOW
//   esp_now_send(broadcastAddress, (uint8_t *)&packet, sizeof(packet));

//   // Delay untuk mencegah pengiriman terlalu cepat
//   delay(150);
// }

#include <esp_now.h>
#include <WiFi.h>

#define pushdown 26 // Ganti dengan pin tombol Anda

uint8_t broadcastAddress[] = {0x10, 0x06, 0x1C, 0x87, 0x07, 0xE4}; //esp2

struct dataPacket {
  char message[50];
};

esp_now_peer_info_t peerInfo;


// Callback saat data terkirim
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  // Inisialisasi WiFi
  WiFi.mode(WIFI_STA);

  // Inisialisasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // Tambahkan peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Atur pin tombol
  pinMode(pushdown, INPUT_PULLUP);
}

void loop() {
    dataPacket packet;

  // Isi pesan
  strcpy(packet.message, "Hello World");

  esp_now_send(broadcastAddress, (uint8_t *) &packet, sizeof(packet));
  Serial.println("Message sent: Hello World");

  delay(500); // Hindari pengiriman terlalu cepat
}
