#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include "PMS.h"
#include "Adafruit_CCS811.h"
#include <Wire.h>
#include <EEPROM.h>

// --- WiFi & Telegram ---
const char* ssid = "*************";
const char* password = "**************";
String botToken = "***************";
String chatID = "***********";

// --- Thingspeak ---
String thingspeakApiKey = "***************";
const char* thingspeakServer = "http://api.thingspeak.com/update";

// --- Sensörler ---
#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
HardwareSerial pmsSerial(2);
PMS pms(pmsSerial);
PMS::DATA data;
Adafruit_CCS811 ccs;

// --- EEPROM & CCS811 ---
#define EEPROM_SIZE 4

// --- Sliding Window Parametreleri ---
#define CCS_WINDOW 15
#define DHT_WINDOW 5
#define PM_WINDOW 5

uint16_t co2Buf[CCS_WINDOW], tvocBuf[CCS_WINDOW];
float tempBuf[DHT_WINDOW], humBuf[DHT_WINDOW];
uint16_t pm25Buf[PM_WINDOW];

int co2Index = 0, dhtIndex = 0, pmIndex = 0;
unsigned long lastCCS = 0, lastDHT = 0, lastPM = 0, lastSend = 0;
unsigned long lastHeatAlert = 0, lastCO2Alert = 0, lastTVOCAlert = 0, lastPMAlert = 0;
const unsigned long alertInterval = 60000;

// --- Heat Index Hesaplama ---
const int tempList[11] = {21, 24, 27, 29, 32, 35, 38, 41, 43, 46, 49};
const int humList[11]  = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
const int heatIndexTable[11][11] = {
  {18, 21, 23, 26, 28, 31, 33, 35, 37, 39, 42},
  {18, 21, 24, 27, 29, 32, 35, 38, 41, 44, 47},
  {19, 23, 28, 31, 34, 37, 41, 44, 47, 49, 54},
  {19, 23, 26, 29, 32, 36, 40, 45, 51, 57, 64},
  {20, 23, 26, 30, 34, 38, 43, 51, 58, 66, -1},
  {21, 24, 27, 31, 36, 42, 49, 57, 66, -1, -1},
  {21, 24, 28, 32, 38, 46, 56, 65, -1, -1, -1},
  {21, 25, 29, 34, 41, 51, 62, -1, -1, -1, -1},
  {22, 26, 30, 36, 45, 58, -1, -1, -1, -1, -1},
  {22, 26, 31, 39, 50, -1, -1, -1, -1, -1, -1},
  {22, 27, 33, 42, -1, -1, -1, -1, -1, -1, -1}
};

int hesaplaHeatIndex(float temp, float hum) {
  int tIdx = 0, hIdx = 0;
  while (tIdx < 10 && temp >= tempList[tIdx + 1]) tIdx++;
  while (hIdx < 10 && hum >= humList[hIdx + 1]) hIdx++;
  int hi = heatIndexTable[hIdx][tIdx];
  return hi == -1 ? 999 : hi;
}


// --- Setup ---
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); Serial.println("WiFi'ye bağlanıyor...");
  }
  Serial.println("WiFi bağlantısı başarılı.");

  dht.begin();
  Wire.begin(21, 22);
  EEPROM.begin(EEPROM_SIZE);
  if (!ccs.begin()) { Serial.println("❌ CCS811 başlatılamadı!"); while (1); }
  ccs.setDriveMode(CCS811_DRIVE_MODE_1SEC);
  delay(5000);

  // EEPROM Baseline
  uint16_t baseline = (EEPROM.read(0) << 8) | EEPROM.read(1);
  if (baseline != 0xFFFF && baseline != 0x0000) {
    ccs.setBaseline(baseline);
    Serial.printf("📦 Baseline yüklendi: 0x%X\n", baseline);
  }

  pmsSerial.begin(9600, SERIAL_8N1, 16, 17);
  pms.wakeUp(); delay(1000); pms.passiveMode();
}

// --- Loop ---
void loop() {
  unsigned long now = millis();

  // CCS811 - 1 saniyede bir
  if (now - lastCCS >= 1000 && ccs.available() && !ccs.readData()) {
    co2Buf[co2Index % CCS_WINDOW] = ccs.geteCO2();
    tvocBuf[co2Index % CCS_WINDOW] = ccs.getTVOC();
    co2Index++;
    lastCCS = now;
  }

  // DHT11 - 3 saniyede bir
  if (now - lastDHT >= 3000) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) tempBuf[dhtIndex % DHT_WINDOW] = t;
    if (!isnan(h)) humBuf[dhtIndex % DHT_WINDOW] = h;
    dhtIndex++;
    lastDHT = now;
  }

  // PMS5003 - 3 saniyede bir
  if (now - lastPM >= 3000) {
    pms.requestRead();
    if (pms.readUntil(data)) {
      pm25Buf[pmIndex % PM_WINDOW] = data.PM_AE_UG_2_5;
      pmIndex++;
    }
    lastPM = now;
  }

  // 15 saniyede bir gönderim ve kontrol
  if (now - lastSend >= 15000 &&
      co2Index >= CCS_WINDOW && dhtIndex >= DHT_WINDOW && pmIndex >= PM_WINDOW) {

    // Ortalama Hesapla
    uint16_t co2 = 0, tvoc = 0, pm = 0;
    float temp = 0, hum = 0;
    for (int i = 0; i < CCS_WINDOW; i++) { co2 += co2Buf[i]; tvoc += tvocBuf[i]; }
    for (int i = 0; i < DHT_WINDOW; i++) { temp += tempBuf[i]; hum += humBuf[i]; }
    for (int i = 0; i < PM_WINDOW; i++) pm += pm25Buf[i];

    co2 /= CCS_WINDOW;
    tvoc /= CCS_WINDOW;
    temp /= DHT_WINDOW;
    hum /= DHT_WINDOW;
    pm /= PM_WINDOW;

    int hi = hesaplaHeatIndex(temp, hum);

    // Thingspeak
    HTTPClient http;
    String url = thingspeakServer;
    url += "?api_key=" + thingspeakApiKey;
    url += "&field1=" + String(temp, 1);
    url += "&field2=" + String(hum, 1);
    url += "&field3=" + String(co2);
    url += "&field4=" + String(tvoc);
    url += "&field5=" + String(pm);
    url += "&field6=" + String(hi);
    http.begin(url);
    http.GET();
    http.end();

    Serial.printf("🌡 %.1f°C | 💧 %.1f%% | HeatIndex: %d | CO₂: %d ppm | TVOC: %d ppb | PM2.5: %d µg/m³\n",
                  temp, hum, hi, co2, tvoc, pm);

    // Telegram Uyarıları
    if (hi >= 41 && now - lastHeatAlert > alertInterval) {
      sendTelegramMessage("🚨 Aşırı sıcak! Heatstroke riski yüksek!\n🌡 Hissedilen Sıcaklık: " + String(hi) + "°C");
      lastHeatAlert = now;
    } else if (hi >= 32 && hi < 41 && now - lastHeatAlert > alertInterval) {
      sendTelegramMessage("⚠️ Güneş çarpması riski olabilir.\n🌡 Hissedilen Sıcaklık: " + String(hi) + "°C");
      lastHeatAlert = now;
    }

    if (co2 >= 2000 && now - lastCO2Alert > alertInterval) {
      sendTelegramMessage("🚨 CO₂ seviyesi KRİTİK! Değer: " + String(co2) + " ppm");
      lastCO2Alert = now;
    } else if (co2 >= 1500 && now - lastCO2Alert > alertInterval) {
      sendTelegramMessage("⚠️ CO₂ seviyesi yüksek! Değer: " + String(co2) + " ppm");
      lastCO2Alert = now;
    }

    if (tvoc >= 1430 && now - lastTVOCAlert > alertInterval) {
      sendTelegramMessage("🚨 TVOC seviyesi KRİTİK! Değer: " + String(tvoc) + " ppb");
      lastTVOCAlert = now;
    } else if (tvoc >= 660 && now - lastTVOCAlert > alertInterval) {
      sendTelegramMessage("⚠️ TVOC seviyesi yüksek! Değer: " + String(tvoc) + " ppb");
      lastTVOCAlert = now;
    }

    if (pm >= 150 && now - lastPMAlert > alertInterval) {
      sendTelegramMessage("🚨 PM2.5 KRİTİK! Değer: " + String(pm) + " µg/m³");
      lastPMAlert = now;
    } else if (pm >= 56 && now - lastPMAlert > alertInterval) {
      sendTelegramMessage("⚠️ PM2.5 yüksek! Değer: " + String(pm) + " µg/m³");
      lastPMAlert = now;
    }

    lastSend = now;
  }
}

// --- Yardımcı Fonksiyonlar ---
void sendTelegramMessage(String msg) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + botToken +
                 "/sendMessage?chat_id=" + chatID + "&text=" + urlencode(msg);
    http.begin(url);
    http.GET();
    http.end();
  }
}

String urlencode(String msg) {
  String encoded = "";
  for (int i = 0; i < msg.length(); i++) {
    char c = msg.charAt(i);
    if (isalnum(c)) encoded += c;
    else {
      char code0 = ((c >> 4) & 0xF) + '0';
      char code1 = (c & 0xF) + '0';
      if (code0 > '9') code0 += 7;
      if (code1 > '9') code1 += 7;
      encoded += '%'; encoded += code0; encoded += code1;
    }
  }
  return encoded;
}
