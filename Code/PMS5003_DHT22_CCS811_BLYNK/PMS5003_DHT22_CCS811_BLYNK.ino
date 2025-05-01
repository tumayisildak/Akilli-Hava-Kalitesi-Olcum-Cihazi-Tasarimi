#include <DHT.h>
#include "PMS.h"
#include "Adafruit_CCS811.h"
#include <Wire.h>

// Blynk Ayarları
#define BLYNK_TEMPLATE_ID "*************";
#define BLYNK_TEMPLATE_NAME "*************";
#define BLYNK_AUTH_TOKEN "*************";

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "*************";  // <-- Wi-Fi ağ adın
char pass[] = "*************";           // <-- Wi-Fi şifren

// DHT22 Ayarları
#define DHTPIN 26
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// PMS5003 Ayarları (RX = GPIO16, TX = GPIO17)
HardwareSerial pmsSerial(2);
PMS pms(pmsSerial);
PMS::DATA data;

// CCS811 Ayarları (I2C: SDA = GPIO21, SCL = GPIO22)
Adafruit_CCS811 ccs;

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("Tüm sensörler başlatılıyor...");

  // Blynk WiFi bağlantısı
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // DHT başlat
  dht.begin();

  // CCS811 başlat
  Wire.begin(21, 22);  // ESP32 I2C pinleri
  if (!ccs.begin()) {
    Serial.println("CCS811 başlatılamadı! Bağlantıyı kontrol edin.");
    while (1);
  }
  delay(5000);  // Kararlılık için bekleme süresi
  Serial.println("CCS811 başlatıldı.");

  // PMS5003 başlat
  pmsSerial.begin(9600, SERIAL_8N1, 16, 17);
  pms.wakeUp();
  delay(1000);
  pms.passiveMode();

  Serial.println("Tüm sensörler hazır.");
}

void loop() {
  Blynk.run();

  // --- DHT22 ---
  float temp = dht.readTemperature();
  delay(200);
  float humidity = dht.readHumidity();

  if (!isnan(temp) && !isnan(humidity)) {
    Serial.print("Sıcaklık: ");
    Serial.print(temp);
    Serial.print(" °C | Nem: ");
    Serial.print(humidity);
    Serial.println(" %");
    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, humidity);
  } else {
    Serial.println("DHT22 verisi okunamadı!");
  }

  // --- CCS811 ---
  if (ccs.available()) {
    if (!ccs.readData()) {
      uint16_t co2 = ccs.geteCO2();
      uint16_t tvoc = ccs.getTVOC();
      Serial.print("CO2: ");
      Serial.print(co2);
      Serial.print(" ppm | TVOC: ");
      Serial.print(tvoc);
      Serial.println(" ppb");
      Blynk.virtualWrite(V2, co2);
      Blynk.virtualWrite(V3, tvoc);
    } else {
      Serial.println("CCS811 veri okuma hatası.");
    }
  }

  // --- PMS5003 ---
  pms.requestRead();
  if (pms.readUntil(data)) {
    Serial.print("PM2.5: ");
    Serial.print(data.PM_AE_UG_2_5);
    Serial.print(" µg/m3 | PM10: ");
    Serial.print(data.PM_AE_UG_10_0);
    Serial.println(" µg/m3");
    Blynk.virtualWrite(V4, data.PM_AE_UG_2_5);
    Blynk.virtualWrite(V5, data.PM_AE_UG_10_0);
  } else {
    Serial.println("PMS5003 verisi alınamadı!");
  }

  Serial.println("=====================================");
  delay(5000);  // Ölçümler arası 5 saniye bekleme
}
