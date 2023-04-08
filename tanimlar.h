#ifndef TANIMLAR_H
#define TANIMLAR_H

////

#define TRUE  1
#define FALSE 0
#define DUMAN_SENSOR_PIN A0
#define HAREKET_SENSOR_PIN D7
#define PAKET_BASLANGIC_1 'M'
#define PAKET_BASLANGIC_2 'B'
#define PAKET_BASLANGIC_V ','

#define OFSET_1000HZ 0
#define OFSET_500HZ  1
#define OFSET_200HZ  2
#define OFSET_100HZ  3
#define OFSET_50HZ   4
#define OFSET_20HZ   5
#define OFSET_10HZ   6
#define OFSET_5HZ    7
#define OFSET_2HZ    8
#define OFSET_1HZ    9

#define SAYAC_1000HZ 1
#define SAYAC_500HZ  2
#define SAYAC_200HZ  5
#define SAYAC_100HZ  10
#define SAYAC_50HZ   20
#define SAYAC_20HZ   50
#define SAYAC_10HZ   100
#define SAYAC_5HZ    200
#define SAYAC_2HZ    500
#define SAYAC_1HZ    1000



typedef struct
{
  unsigned long int sayac_1ms;  
  const char *kullanici_id;
  const char *kullanici_sifre;
  const char *wifi_id;
  const char *wifi_sifre;
  const char *cihaz_tur;
  const char *cihaz_id;
  const char *sensor_deger;
  int cihaz_konfigurasyon_pkt_geldi;
  int cihaz_konfigurasyon_wifi_ilk_baglanti;

}GL_t;


typedef struct
{
    int           paket_turu;
    unsigned char veri_paket_a[255];
    int           veri_boyutu;
    
}mb_paketi_t;








#endif
