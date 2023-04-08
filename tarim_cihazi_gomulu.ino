#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include "paket_turleri.h"
#include "tanimlar.h"



GL_t GL_st = {0};
GL_t GL_eeprom_st = {0};
mb_paketi_t mb_paketi_st = {0};



ESP8266WiFiMulti WiFiMulti;
WiFiClient  client;
WiFiUDP     Udp;
WiFiServer  server(80);

IPAddress   local_IP(192,168,3,1);
IPAddress   gateway(192,168,3,1);
IPAddress   subnet(255,255,255,0);

// Bunlar sabit tanimlara alinacak
const char* sunucuAdres = "185.95.164.200";
const int   sunucuPort  = 11001;
const char* ssidAP        = "MUHBIR";
const char* passwordAP    = "1234567890";
unsigned int    udp_port = 11000;
         
// GL sifirla
void gl_sifirla(void)

  int index = 0;
  
  char *chr = (char*)&GL_st;
    
  for(index=0; index< sizeof(GL_st); index++)
  {
    chr[index] = 0;    
  }

}

// Udp paket gonderme Fonksiyonu
void udp_paket_gonder(IPAddress gonderilecek_ip_addr, const char* p_gonderilecek_veri){
   Udp.beginPacket(gonderilecek_ip_addr, udp_port);
   Udp.write(p_gonderilecek_veri,strlen(p_gonderilecek_veri)+1);
   Udp.endPacket();

}//

// AccessPoint olusturma fonksiyonu
void accessPointOlustur(){
  Serial.print("AP olusturuluyor");
  Serial.println(WiFi.softAP(ssidAP,passwordAP) ? "AP basarili" : "AP basarisiz");

  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  delay(200);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();

  delay(200);
}
//

// AccessPoint kapatma fonksiyonu
void accessPointKapat() {
  WiFi.softAPdisconnect(true);
}

// Wifi baglanma fonksiyonu
void wifiBaglan(GL_t *GL_st) {

  Serial.println();
  Serial.print("Bağlanılıyor: ");

  //GL_st->wifi_id = "SUPERONLINE_WiFi_5568" ;
  //GL_st->wifi_sifre ="BuzParmak26.";

  static unsigned long int sayac = 0;  

  if(TRUE == sayac %10)
  {
    // 1 kere gondermek gerekiyor.
    WiFi.begin(GL_st->wifi_id, GL_st->wifi_sifre);
    
  }

  sayac++;

}
/////

//Wifi baglantisi kesildi mi diye kontrol
void wifiBaglantisiKontrol(GL_t *GL_st)
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("WiFi bağlantısı başarısız oldu");
    wifiBaglan(GL_st);
  }
  else
  {
    Serial.println("");
    Serial.println("WiFi bağlantısı yapıldı");
    Serial.print("IP adresi: ");
    Serial.println(WiFi.localIP());

    tcpSunucuBaglantisiKontrol();
  } 


}

// TCP Sunucusuna baglanma fonksiyonu
void tcpSunucusunaBaglan() {

  if (client.connect(sunucuAdres, sunucuPort)) {
    Serial.println("Bağlantı başarılı");
  } else {
    Serial.print("Bağlantı hatası");
  }  
  
} //////

// TCP Sunucusuna baglanti kontrol
void tcpSunucuBaglantisiKontrol() {
  // Sunucu bağlantısı kesildi mi diye kontrol
  if (!client.connected()) {
    Serial.println();
    Serial.println("Sunucu bağlantısı kesildi");
    Serial.println("Yeniden bağlanılıyor...");
    tcpSunucusunaBaglan();
  }
}
/////

// Paket cozme fonksiyonu
void paket_coz(mb_paketi_t *mp_paketi_st){

    Serial.println(mp_paketi_st->paket_turu);
    
    switch (mp_paketi_st->paket_turu)
    {
      
        case(YOKLAMA):
        {
          Serial.println("yoklama_paket_gnoderme");  
          
          paket_olustur_gonder_yoklama(); 

          break;
          
        }
        case(KULLANICI_KONFIGURASYON):
        {
            GL_st.kullanici_id = ayirac;

            ayirac = strtok(NULL, ",");

            GL_st.kullanici_sifre = ayirac;

            break;
        }
        case(CIHAZ_KONFIGURASYON):
        {
            int n = mp_paketi_st->veri_boyutu; 
           
            // virgüllerin sayısını hesapla
            int virgul_sayac = 0;
            for (int i = 0; i < n; i++) {
                if (mp_paketi_st->veri_paket_a[i] == 0x2c) { // 0x2c: virgülün hex kodu
                    virgul_sayac++;
                }
            }

            // virgüllere göre ayırma işlemi yap
            if (virgul_sayac == 4) {
                // virgül sayısı 4 ise 5 parçaya ayır
                char* ayirac = strtok((char*)mp_paketi_st->veri_paket_a, ",");
                GL_st.kullanici_id = ayirac;
                ayirac = strtok(NULL, ",");
                GL_st.wifi_id = ayirac;
                ayirac = strtok(NULL, ",");
                GL_st.wifi_sifre = ayirac;
                ayirac = strtok(NULL, ",");
                GL_st.cihaz_id = ayirac;
            }


            break;
        }
        default:
        {
            break;
        }
      

    }

}
////


// Paket yakalama fonksiyonu
void paket_yakalama_makine(unsigned char *yakalanan_veriler,int len){
  
    static int paket_yakalama_makinesi_u8 = 0;
    static int j = 0;

    for (int i = 0; i < len; i++)
    { 

      switch (paket_yakalama_makinesi_u8)
      {
          case(PAKET_SENKRON_1):
          {          

              if (PAKET_BASLANGIC_1 == yakalanan_veriler[i])
              {
                  paket_yakalama_makinesi_u8 = PAKET_SENKRON_2;

              }

              j = 0;
              break;

          }
          case(PAKET_SENKRON_2):
          {
              if (PAKET_BASLANGIC_2 == yakalanan_veriler[i])
              {
                  paket_yakalama_makinesi_u8 = PAKET_TURU;
              }
              else
              {
                  paket_yakalama_makinesi_u8 = PAKET_SENKRON_1;
              }

              j = 0;
              break;
          }

          case(PAKET_TURU):
          {

              mb_paketi_st.paket_turu = yakalanan_veriler[i];


              paket_yakalama_makinesi_u8 = PAKET_VERI_BOYUTU;

              j = 0;
              break;

          }
          
          case(PAKET_VERI_BOYUTU):
          {

              mb_paketi_st.veri_boyutu = yakalanan_veriler[i];


              paket_yakalama_makinesi_u8 = PAKET_VERI;

              j = 0;
              break;

          }
          case(PAKET_VERI):
          {
              
              mb_paketi_st.veri_paket_a[j] = yakalanan_veriler[i];
              j++;

              if (mb_paketi_st.veri_boyutu == j )
              {
                  paket_yakalama_makinesi_u8 = PAKET_SENKRON_1;

                  paket_coz(&mb_paketi_st);
              }
                        
              
              break;

          }
          default:
          {
              j = 0;
              break;
          }
      
      }
    }
}
////

// Paket olustur gonder cihaz durum fonksiyonu
void  paket_olustur_gonder_cihaz_durum(GL_t *GL_st){

  if (client.connected()) {

    char gonderilecek_veri[255] = { 0 };

    const char* paket_baslangic = "MB,4"; 

    GL_st->cihaz_tur    = "HAREKET";
    GL_st->sensor_deger = "1";

    strcpy(gonderilecek_veri, paket_baslangic);
    strcat(gonderilecek_veri, ",");
    strcat(gonderilecek_veri, GL_st->kullanici_id);
    strcat(gonderilecek_veri, ",");
    strcat(gonderilecek_veri, GL_st->cihaz_tur);
    strcat(gonderilecek_veri, ",");
    strcat(gonderilecek_veri, GL_st->cihaz_id);
    strcat(gonderilecek_veri, ",");
    strcat(gonderilecek_veri, GL_st->sensor_deger);

    printf("Gonderilecek Veri: %s\n", gonderilecek_veri);

    client.write(gonderilecek_veri);
    
  }
  

    
} 
///

// Paket olustur gonder cihaz durum fonksiyonu
void paket_olustur_gonder_yoklama(){
  
  const char* paket_baslangic = "MB,0,0"; 

  printf("Gonderilecek Veri: %s\n", paket_baslangic);

  udp_paket_gonder(Udp.remoteIP(), paket_baslangic); 
    
} 


// Udp paket yakalama fonksiyonu
void udp_paket_yakala_gonder(){

  unsigned char mobil_yakalanan_veri[255] = {0};
  
  if (Udp.parsePacket()){
    Serial.println("paket geldi");
    int len = Udp.read(mobil_yakalanan_veri,255);
    Serial.println(len);
    if (len > 0)
    {
      paket_yakalama_makine(mobil_yakalanan_veri,len);

      Serial.println(mobil_yakalanan_veri);
      Serial.println(Udp.remoteIP());                   
    }
  
  }
}
///

// Eeprom Yaz fonksiyonu
/*
void eepromYaz(GL_t *GL_st) {
    EEPROM.put(0, *GL_st);
    EEPROM.commit();
}

// Eeprom Oku fonksiyonu
void eepromOku(GL_t *GL_eeprom_st) {
    EEPROM.get(0, *GL_eeprom_st);
}

// Eeprom Sil fonksiyonu
void eeprom_sil() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.update(i, 0);
  }
}
*/


void setup(void)
{


  GL_st.kullanici_id =  "MRC";
  GL_st.cihaz_tur    =  "DEPREM";
  GL_st.cihaz_id     =  "OTURMAODASI";
  GL_st.sensor_deger =  "1";
  GL_st.wifi_id      =  "Zyxel_E2E1";
  GL_st.wifi_sifre   =  "MHH4L73JHF";
  

  gl_sifirla();

	Serial.begin(9600);

	Udp.begin(udp_port);

	WiFi.mode(WIFI_AP_STA);
  
  Serial.print("AP olusturuluyor");
  Serial.println(WiFi.softAP(ssidAP,passwordAP) ? "AP basarili" : "AP basarisiz");

  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  delay(200);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();

  delay(200);
  // Access Point olustur
  accessPointOlustur();

  // Wifi baglan
 // wifiBaglan(&GL_st);

  // TCP sunucusuna baglan
  //tcpSunucusunaBaglan();


}

void loop(void)
{

  GL_st.sayac_1ms = millis();  

  if( OFSET_1000HZ == (GL_st.sayac_1ms % SAYAC_1000HZ)  )
  {    

  }

  if( OFSET_500HZ == (GL_st.sayac_1ms % SAYAC_500HZ)  )
  {    
 
  }

  if( OFSET_200HZ == (GL_st.sayac_1ms % SAYAC_200HZ)  )
  {    

  }

  if( OFSET_100HZ == (GL_st.sayac_1ms % SAYAC_100HZ)  )
  {    
 
  }

  if( OFSET_50HZ == (GL_st.sayac_1ms % SAYAC_50HZ)  )
  {
 

    
  }





  if( OFSET_20HZ == (GL_st.sayac_1ms % SAYAC_20HZ)  )
  {    
    

    
  }


  if( OFSET_10HZ == (GL_st.sayac_1ms % SAYAC_10HZ)  )
  {   
    

        

  }


  if( OFSET_5HZ == (GL_st.sayac_1ms % SAYAC_5HZ)  )
  {    

  }


  if( OFSET_2HZ == (GL_st.sayac_1ms % SAYAC_2HZ)  )
  {
    
    //Eger mobilden yoklama paketi geldiyse mobile tekrar gonderilir.
    udp_paket_yakala_gonder();
  
  
        
  }

  if( OFSET_1HZ == (GL_st.sayac_1ms % SAYAC_1HZ)  )
  {
    static unsigned long sayac = 0 ;   

    wifiBaglantisiKontrol(&GL_st);


    if( (TRUE          == GL_st.cihaz_konfigurasyon_pkt_geldi) &&
        (WiFi.status() == WL_CONNECTED                        )  )
    {

      const char* paket_baslangic = "MB,3,konfigurasyon_basarili"; 

      printf("Gonderilecek Veri: %s\n", paket_baslangic);

      udp_paket_gonder(Udp.remoteIP(), paket_baslangic); 


      GL_st.cihaz_konfigurasyon_pkt_geldi = FALSE;

  
    }


    paket_olustur_gonder_cihaz_durum(&GL_st);

    



 

  }
  
  



  //Sunucuya paket gonderme

  //paket_olustur_gonder_cihaz_durum(&GL_st);
  // Sunucu bağlantısı kesildi mi diye kontrol
  //tcpSunucuBaglantisiKontrol();

  // Wifi bağlantısı kesildi mi diye kontrol
  //wifiBaglantisiKontrol(&GL_st); 


   


  
}
