#include "dht_module.h"
#include "config.h" // Nạp file cấu hình chân cắm
#include <DHT.h>

// Khởi tạo đối tượng DHT
DHT dht(DHT_PIN, DHT_TYPE);

// Khởi tạo cảm biến DHT
void setupDHT() {
    dht.begin();
    Serial.println("DHT Module OK!");
}

// Đọc giá trị nhiệt độ
float getTemperature() {
    return dht.readTemperature();
}

// Đọc giá trị độ ẩm
float getHumidity() {
    return dht.readHumidity();
}
