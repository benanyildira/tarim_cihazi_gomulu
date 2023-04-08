#ifndef PAKET_TURLERI_H
#define PAKET_TURLERI_H


#define PAKET_BASLANGIC_1  77   //  'M'
#define PAKET_BASLANGIC_2  66   //  'B'
#define PAKET_BASLANGIC_V  44   //  ','

enum paket_yakalama_yapisi
{
    PAKET_SENKRON_1 ,   //  Baslangic1
    PAKET_SENKRON_2,    //  Baslangic2
    PAKET_TURU,         //  Tur
    PAKET_VERI_BOYUTU,  //  Veri Boyutu
    PAKET_VERI          //  Veri
};

enum paket_turleri
{
    YOKLAMA,
    KULLANICI_KONFIGURASYON,   // KULLANICIADI,KULLANICISIFRE
    KULLANICI_GIRIS,           // KULLANICIADI, KULLANICISIFRE
    CIHAZ_KONFIGURASYON,       // WIFIID,WIFISIFRE,CIHAZID
    CIHAZ_DURUM,               // KULLANICIADI, CIHAZTUR, CIHAZID, SENSORDEGER
    MOBIL_KIMLIK,              // KULLANICIADI
    MOBIL_SORGU_GECMIS_DURUM,  // KULLANICIADI
};







#endif