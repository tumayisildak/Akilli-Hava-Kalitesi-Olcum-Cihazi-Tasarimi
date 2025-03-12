<p align="center"><h3 align="center">Akıllı Hava Kalitesi Ölçüm Cihazı Tasarımı</h3></p>

## **1. Proje Konusu**  
Bu proje hava kalitesini ölçmek ve kullanıcıyı zararlı durumlarda uyarmak
için tasarlanmış bir sistemdir. Gerekli sensörler kullanılarak veri toplanır
ve mikrodenetleyici yardımıyla işlenir. Veriler Wi-Fi üzerinden bir platforma aktarılır, böylece kullanıcılar gerçek zamanlı olarak sonuçlara
erişebilir. Hava kalitesi kritik seviyelere ulaştığında sistem sesli uyarı ve bildirimler ile kullanıcıları anlık bilgilendirerek sağlık ve konfor için pratik bir çözüm sunar. 

## **2. Proje Hedefleri**  
- Hava kalitesini belirleyen çeşitli sensörler ile ortam verilerinin toplanması  
- Verilerin mikrodenetleyici tarafından işlenmesi ve analiz edilmesi  
- Kablosuz bağlantı (Wi-Fi veya Bluetooth) ile verilerin bir platforma aktarılması  
- Kullanıcının hava kalitesini görüntüleyebilmesi
- Opsiyonel olarak, yerel veri önbellekleme desteği ekleyerek internet kesintilerinde veri kaybını önlemek  

## **3. Tahmini Zaman Çizelgesi**  

| Görev | Süre (Gün) | Açıklama |
|--------|----------|-----------|
| Sensör araştırması ve seçimi | 7 | Kullanılacak sensörlerin belirlenmesi |
| Donanım montajı ve testler | 10 | Devre kurulumu ve bileşenlerin test edilmesi |
| Mikrodenetleyici yazılım geliştirme | 14 | ESP32 kodlarının yazılması ve sensörlerden veri okunması |
| Kablosuz iletişim entegrasyonu | 7 | Wi-Fi/Bluetooth ile veri aktarımının sağlanması |
| Bulut/veri görselleştirme entegrasyonu | 7 | Verilerin uzaktan erişilebilir hale getirilmesi |
| Test ve hata ayıklama | 5 | Sistem güvenilirliğinin test edilmesi |
| Son dokümantasyon ve raporlama | 5 | Proje raporlarının hazırlanması |

## **4. Kaynak Planlaması**  
- **Proje Sahibi:** Tümay Işıldak  
- **Donanım:** ESP32, CO2, VOC, PM2.5, sıcaklık/nem sensörleri, güç kaynağı 
- **Yazılım:** Arduino IDE, Python, React (opsiyonel), Blynk veya alternatif platform  

### **Tahmini Maliyet Tablosu**  

| **Bileşen**               | **Adet** | **Fiyat (₺)** |
|--------------------------|--------|----------------|
| ESP32   | 1      | 275            |
| CO2 Sensörü (MH-Z19B)   | 1      | 1600            |
| VOC Sensörü (CCS811)    | 1      | 600           |
| PM2.5 Sensörü (PMS5003)  | 1      | 540            |
| Sıcaklık & Nem Sensörü (DHT22) | 1  | 65            |
| Li-Po Batarya          | 1      | 400            |
| Kutu ve Montaj Malzemeleri    | 1  | 300         |
| **Toplam Maliyet**       | -      | **₺3780**       | 


## **5. Risk Analizi**  

| Risk | Olasılık | Etki | Risk Skoru | Çözüm |
|------|---------|--------|---------|--------|
| Sensör doğruluğunda sapmalar | 3 (Orta) | 4 (Yüksek) | 12 (Yüksek Risk) | Kalibrasyon yapılmalı, hassas sensörler tercih edilmeli |
| Wi-Fi kesintileri | 4 (Yüksek) | 3 (Orta) | 12 (Yüksek Risk) | Bluetooth veya yerel önbellekleme alternatifleri değerlendirilmeli |
| Pil ömrü ve güç tüketimi | 3 (Orta) | 3 (Orta) | 12 (Orta Risk) | Düşük güç tüketimli sensörler tercih edilmeli, enerji verimli algoritmalar kullanılmalı |

## **6. Ticari Potansiyel**  
Bu proje, hava kalitesini takip etmek isteyen bireyler, kamu kurumları ve işletmeler için kullanılabilir. Potansiyel kullanım alanları:  
- Evler, ofisler ve okullar  
- Hastaneler
- Fabrikalar, üretim tesisleri ve atölyeler  
- Akıllı şehir projeleri ve çevre izleme sistemleri  
