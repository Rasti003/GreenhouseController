#include <Adafruit_GFX.h>   ///biblioteki tft
#include <Adafruit_ST7735.h> ///biblioteki tft

#include <SPI.h>                          ////biblioteki komunikacyjne
#include <Wire.h>
#include <EtherCard.h>
#include <EEPROM.h>                       // biblioteka EEPROM
#include "DHT.h"                          // biblioteka DHT
#include <DS3231.h>                       /// biblioteka RTC

#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8                      // okreslenie pinow dla ekranu 
#define TFT_SCLK   52
#define TFT_MOSI   51

#define DHTPIN 11                         // numer pinu sygnalowego czujnika
#define DHTTYPE DHT11                     // typ czujnika (DHT11). Jezeli posiadamy DHT22 wybieramy DHT22

#define ETHERNET_CS  3                     // pin cs dla karty ethernet
#define FEED "1506222864"                  // NUMER KANALU SERWERA
#define APIKEY "lo47yOMnW0ON1RAhyzApiT7b3Q5GOeZ0eLsPXQBCeHEXns0U" // klucz API

static byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 }; // ustalenie adresu mac karty sieciowej
//static byte myip[] = { 192,168,88,203 };                    // Ustawienie statycznego IP

const char website[] PROGMEM = "api.xively.com";              // okreslenie aresu serwera 

byte Ethernet::buffer[700];     // wielkosc buforu na dane
uint32_t timer;
Stash stash;    // klasa stah z wartisciami do weslania
byte session;   // zmienna bitowa z danymi na serwer
int res = 0;    // licznik okresu polaczenia
int f = 0 ;     //znacznik stanu polaczenia


DHT dht(DHTPIN, DHTTYPE); // konfiguracj czujbika DHT

DS3231 clock;             // konfiracja nazwnictwa dla  modulu RTC
RTCDateTime dt;


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);    // konfiguracja bibliotek ekranu


unsigned long tft_odswiez = 2000;       //predkosc odswiezania ekranu
unsigned long tft_odswiezTMP = 0;

unsigned long serial_odswiez = 5000;    //okres wysyłania informacji na serial
unsigned long serial_odswiezTMP = 0;

int tempOPT = 22;        // wartosc startowa temperatury
int wgleby_zad = 10;     // wartosc startowa nawodnienia
int tempON = 0;          // temperatura maksymalna // zalaczeniowa wentylatora                 /
int tempOFF = 0;         // temperatura minimalna  // wylaczeniowa wentylatora
int wiatrak = 7;         // wlacznik wentylatora
int swiatloON = 6;       // pin wlacznika swiatla
int swiatlo = 2 ;        // pin czujnika swiatla
int gleba = 1 ;          // pin odczytu wilgotnosci gleby;
int pompa = 4;           // wlacznik pompy nawadniajacej

float t;                 //odczyt temperatury dht11
float h;                 // odczyt wilgotnosci powietrza

int i = 0;               // zmienna pomocnicza

int swiatlo_godz_wyl;      // godzina wylaczenia swiatla
int swiatloCZAS ;          // liczba godzin swiecenia
int swiatlo_timeH;         // zmienne pomocnicze
int swiatlo_timeM;         // J/w
int swiatlo_stan;          // stan czujnika oswietlenia


int wgleby = 0 ;          // pobrana wartosc z wilgotosci gleby

int pompa_tmp = 0;        // zmienna pomocnicza



char serial_odczyt ;        //zmienna na potrzeby odczytywania wartosci wpisanych w serial monitor
String serial_odczyt2 = ""; // j/w

///////////////////////////////////////////////////////////////////
void setup(void) {
  

  pinMode(wiatrak, OUTPUT);                  //
  pinMode(swiatloON, OUTPUT);               //// ustawianie pinow jako piny wejsciowe/wyjsciowe
  pinMode(swiatlo, INPUT);                  // podciagniecie wewnetrznie pinu do masy zapobiega smieceniu na wejsciu (wiszeniu w powietrzu)
  pinMode(pompa, OUTPUT);
  pinMode(gleba, INPUT);

  Serial.begin(57600);                      // inicjalizacja i okreslenie predkosc komunikacji na serial 
  delay(2000);                              // czas na uruchominie komunikacji
  
  informacje_startowe();                    // wyswietlanie informacji poczatkowych na ekranie
  delay(2000);
  dht.begin();                               // inicjalizacja czujnika dht

  clock.begin();                             // inicializacja RTC
  //clock.setDateTime(__DATE__, __TIME__);   // ustawienie czasu zgodnego z data kompilacji

  attachInterrupt(0, przerwanie, FALLING);   //inicjalizacja obslugi przerwania gdy stan zmieni sie z wysokiego (brak slonca) na niski (jest slonce)

  wgleby_zad = EEPROM.read(2);               //
  tempOPT = EEPROM.read(1);                  //// odczyt zapisanych danych w pamieci EEPROM 
  swiatloCZAS = EEPROM.read(3);              ////
  swiatlo_godz_wyl =EEPROM.read(4);          //

  Ethernet_uruchom();                        // konfiguracja polaczenia sieciowego  
  informacje_startowe();                     // aktualizacja danych na ekranie
delay(2000);

}


/////////////////////////////////////////////////////////////////////////


void loop() {
 
  tempON = tempOPT + 2; ///pid temperaturowy temp gorna
  tempOFF = tempOPT - 2; // pid temperaturowy temp dolna
  

  int t = dht.readTemperature();            //odczyt temperatury dht11
  int h = dht.readHumidity();               // odczyt wilgotnosci powietrza

  doswietl();
  sterTEMP(t);
  w_gleby();
  dane_wyslij(h, t);                        // wywolanie funkcji
  RS232(h, t);
  LCD(h, t);
  
}





