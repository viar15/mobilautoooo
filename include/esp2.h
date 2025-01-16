// #include <esp_now.h>
// #include <WiFi.h>
// #include <Wire.h>
// #include <math.h>

// // PIN dan Sensor
// #define pushdown 26 // Tombol
// #define trigPin 5   // Ultrasonik trigger
// #define echoPin 18  // Ultrasonik echo

// const int MPU_addr = 0x68; // Alamat I2C MPU6050

// // Alamat MAC tujuan (sesuaikan dengan perangkat penerima)
// uint8_t broadcastAddress[] = {0x10, 0x06, 0x1C, 0x87, 0x07, 0xE4};

// // Struktur data untuk dikirim
// struct SensorData {
//   float roll;
//   float pitch;
// };

// SensorData sensorData; // Variabel untuk menyimpan data sensor

// esp_now_peer_info_t peerInfo;

// // Callback untuk mengonfirmasi pengiriman data
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("Last Packet Send Status: ");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }

// void setup() {
//   // Inisialisasi Serial Monitor
//   Serial.begin(115200);

//   // Inisialisasi WiFi
//   WiFi.mode(WIFI_STA);

//   // Inisialisasi ESP-NOW
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }

//   // Tambahkan callback untuk pengiriman data
//   esp_now_register_send_cb(OnDataSent);

//   // Tambahkan perangkat peer
//   memcpy(peerInfo.peer_addr, broadcastAddress, 6);
//   peerInfo.channel = 0;
//   peerInfo.encrypt = false;
//   if (esp_now_add_peer(&peerInfo) != ESP_OK) {
//     Serial.println("Failed to add peer");
//     return;
//   }

//   // Inisialisasi tombol dan sensor
//   pinMode(pushdown, INPUT_PULLUP);
//   pinMode(trigPin, OUTPUT);
//   pinMode(echoPin, INPUT);

//   // Inisialisasi I2C untuk MPU6050
//   Wire.begin();
//   MPU_Init();
// }

// void loop() {
//   // Variabel waktu untuk menghitung pitch, roll, dan yaw
//   static float previousTime = 0;
//   float currentTime = millis();
//   float elapsedTime = (currentTime - previousTime) / 1000.0; // Konversi ke detik
//   previousTime = currentTime;

//   // Membaca data akselerometer dari MPU6050
//   int16_t AcX, AcY, AcZ;
//   Wire.beginTransmission(MPU_addr);
//   Wire.write(0x3B); // Alamat awal data akselerometer
//   Wire.endTransmission(false);
//   Wire.requestFrom(MPU_addr, 6, true);
//   AcX = Wire.read() << 8 | Wire.read();
//   AcY = Wire.read() << 8 | Wire.read();
//   AcZ = Wire.read() << 8 | Wire.read();

//   // Konversi data akselerometer ke pitch dan roll
//   float accX = AcX / 16384.0;
//   float accY = AcY / 16384.0;
//   float accZ = AcZ / 16384.0;

//   sensorData.pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * 180.0 / M_PI;
//   sensorData.roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * 180.0 / M_PI;

//   // Menampilkan data ke Serial Monitor
//   Serial.print("Roll: ");
//   Serial.print(sensorData.roll);
//   Serial.print(" | Pitch: ");
//   Serial.print(sensorData.pitch);
//   // Serial.print(" | Distance: ");
//   // Serial.print(sensorData.distance);
//   // Serial.println(" cm");

//   // Mengirim data melalui ESP-NOW
//   esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&sensorData, sizeof(sensorData));
//   if (result == ESP_OK) {
//     Serial.println("Data berhasil dikirim");
//   } else {
//     Serial.println("Gagal mengirim data");
//   }

//   delay(1000); // Kirim data setiap 1 detik
// }

// void MPU_Init() {
//   Wire.beginTransmission(MPU_addr);
//   Wire.write(0x6B); // PWR_MGMT_1 register
//   Wire.write(0);    // Set ke 0 untuk mengaktifkan MPU6050
//   Wire.endTransmission(true);
// }

#include <WiFi.h>
#include <esp_now.h>

// Struktur data yang sama dengan pengirim
struct SensorData {
    float roll;
    float pitch;
    float distance;
};

SensorData receivedData;  // Variabel untuk menyimpan data yang diterima

// Callback untuk menerima data
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));

    // Menampilkan data ke Serial Monitor
    Serial.println("Data diterima:");
    Serial.print("Roll: ");
    Serial.print(receivedData.roll);
    Serial.print(" | Pitch: ");
    Serial.print(receivedData.pitch);
    Serial.print(" | Distance: ");
    Serial.print(receivedData.distance);
    Serial.println(" cm");
}

void setup() {
    // Inisialisasi Serial Monitor
    Serial.begin(115200);

    // Inisialisasi WiFi sebagai Station (STA)
    WiFi.mode(WIFI_STA);

    // Inisialisasi ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Mendaftarkan callback untuk menerima data
    esp_now_register_recv_cb(OnDataRecv);

    Serial.println("ESP-NOW Receiver siap menerima data.");
}

void loop() {
    // Tidak perlu melakukan apa-apa di loop utama
    delay(1000);  // Tambahkan delay untuk debugging
}