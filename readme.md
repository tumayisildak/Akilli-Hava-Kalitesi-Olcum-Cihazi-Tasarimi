<p align="center"><h3 align="center">Akıllı Hava Kalitesi Ölçüm Cihazı Tasarımı</h3></p>

---

## 📌 Akıllı Hava Kalitesi Ölçüm Cihazı Tasarımı
Bu proje hava kalitesini ölçmek ve kullanıcıyı zararlı durumlarda uyarmak
için tasarlanmış bir sistemdir. Gerekli sensörler kullanılarak veri toplanır
ve mikrodenetleyici yardımıyla işlenir. Veriler Wi-Fi üzerinden bir platforma aktarılır, böylece kullanıcılar gerçek zamanlı olarak sonuçlara
erişebilir. Hava kalitesi kritik seviyelere ulaştığında sistem sesli uyarı ve bildirimler ile kullanıcıları anlık bilgilendirerek sağlık ve konfor için pratik bir çözüm sunar.

---

## 📋 Proje Gereksinimleri  
Bu projeyi çalıştırabilmek için aşağıdaki donanım ve yazılımlara ihtiyacınız vardır:  

### Donanım Gereksinimleri  
- **ESP32 Mikrodenetleyici**  
- **Sensörler**  
  - CO2 Sensörü
  - VOC Sensörü   
  - PM2.5 Sensörü  
  - Nem & Sıcaklık Sensörü 

### Yazılım Gereksinimleri  
- **Arduino IDE** (ESP32 için)  
- **Platform SDK** (Örneğin: Thingspeak vb.)  
- **Gerekli Kütüphaneler** (Arduino için: `Adafruit_Sensor`, `DHT`, `WiFiClient`,`HTTPClient`)  

---

## 🚀 Projeyi Çalıştırma  
1. **Bağlantıları Yapın**  
   - Sensörleri ESP32'ye bağlayın.  
   - Wi-Fi ile bağlantıyı test edin.  

2. **Yazılım Kurulumu**  
   - Arduino IDE’ye ESP32 kart desteğini ekleyin.  
   - Gerekli kütüphaneleri yükleyin.  
   - Dosyada bulunan sansürlü yerleri kendinize uygun bilgilerle doldurun.  

3. **ESP32’ye Kodu Yükleyin**  
   - Kodu ESP32’ye yükleyin.  
   - Seri monitörden hata ayıklayın.  

4. **Veri Gönderimi ve Görselleştirme**  
   - Verileri bir platforma gönderin (Örn: ThingSpeak).  
   - Wweb uygulaması ile verileri görüntüleyin.  

---

## 📜 Proje Lisans Bilgileri  
Bu proje [MIT Lisansı](LICENSE) kapsamında dağıtılmaktadır. Kullanım ve dağıtım koşulları için lisans dosyasını inceleyebilirsiniz.  

---

## 🔍 Proje Anahtar Kelimeleri  
`ESP32`, `Hava Kalitesi`, `IoT`, `Sensör`, `ThingSpeak`, `Wi-Fi`, `CO2`, `VOC`, `PM2.5`, `Akıllı Cihaz`, `Nem ve Sıcaklık`  

---
