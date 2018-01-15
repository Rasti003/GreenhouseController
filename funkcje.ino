void ustawienia_serial() {                  //funkcjia wyswietlania menu konfiguracyjnego

  Serial.println("///////////////////////////////////////");
  Serial.println("          ====USTAWIENIA====");
  Serial.println("    Napisz txx w miejscu xx podaj temperature");
  Serial.println("    Napisz nxx w miejscu xx podaj nawodnienie");
  Serial.println("    Napisz cxx w miejscu xx podaj czas nswietlania w H");

}


void parametry_serial (int h, int t) {        //funkcja wyswietlania parametrow pracy
  Serial.println("///////////////////////////////////////");
  Serial.println("      [DANE Z CZUJNIKOW]");
  Serial.print("  temperatura: ");  Serial.print(t); Serial.print("'C");                //

  Serial.print("     ");                                                                //// wyswietlanie aktualnych wartosci pobranych z czujnikow
  Serial.print("wilgotnosc: "); Serial.print(h); Serial.print("%");                     ////

  Serial.print("     ");
  Serial.print("Nawodnienie: "); Serial.print(wgleby);  Serial.println("%");            //
  Serial.println(" ");

  Serial.println ("      [USTAWIENIA PODSTAWOWE]");
  Serial.print ("Temperatura: "); Serial.print (tempOPT);       Serial.print ("C'     ");             //
  Serial.print("nawodnienie: "); Serial.print(wgleby_zad);      Serial.println("%");                  ////wyswietlanie zadanych wartosci
  Serial.print("Minimalny czas ze swiatlem: "); Serial.print(swiatloCZAS); Serial.println(" godzin"); //

}



void RS232 (int h, int t) {

  ustawienia(h, t);                             // uruchomienie funkcji ustawien
  unsigned long time = millis();                // pobieranie czasu od startu urzadzenia
  if ( serial_odswiezTMP  < time) {
    serial_odswiezTMP = time +  serial_odswiez; // konfiguracja czasu wyswietlania informacji na serial monitorze
    Serial.println(" ");
    Serial.println("/////////////////////////////////////////////////");
    Serial.println("Napisz 'u' zeby ustawic");
    Serial.println("Napisz 'p' zeby wyswietlic parametry urzadzenia");
  }
}
///////////////////////////////////////////////////////////////////////

void ustawienia(int h, int t) {

  serial_odczyt = Serial.read();               // odczytywanie znaku odniesienia
  switch (serial_odczyt) {
    case 'p':                                  //wybor znaku odniesienia "p" - wyswietlanie parametrĂłw urzadzenia
      parametry_serial( h, t);                 //wyswietlanie parametrĂłw urzadzenia
      break;

    case 'u':                                   //wybor znaku odniesienia "u" - wyswietlanie instrukcji konfiguracji
      ustawienia_serial();                      //wyswietlanie instrukcji konfiguracji
      break;

    case 't':                                   //wybor znaku odniesienia "t" - temperatura
      while (Serial.available())                // pentla odczytujaca wartosci po znaku odniesienia
      {
        char znak = (char)Serial.read();        // odczytywanie kolejno wartosci
        serial_odczyt2 += znak;

        tempOPT = serial_odczyt2.toInt();       // konwersja odczytanej wartosci do typu int
        EEPROM.write(1, tempOPT);               //zapis nowej wartosci temperatury w EEPROM
      }

      Serial.println("          ====DANE ZAPISANE===");
      Serial.println("");

      serial_odczyt2 = "";                      //czyszczenie buforu serial monitora
      break;

    case 'n':                                   //wybor znaku odniesienia "n" - nawodnine gleby

      while (Serial.available())                // pentla odczytujaca wartosci po znaku odniesienia
      {
        char znak = (char)Serial.read();        // odczytywanie kolejno wartosci
        serial_odczyt2 += znak;

        wgleby_zad = serial_odczyt2.toInt();    // konwersja odczytanej wartosci do typu int
        EEPROM.write(2,  wgleby_zad);           //zapis nowej wartosci wilgotnosci w EEPROM
      }

      Serial.println("          ====DANE ZAPISANE===");
      Serial.println("");

      serial_odczyt2 = "";                      //czyszczenie buforu serial monitora
      break;

    case 'c' :                                  //wybor znaku odniesienia "c" - czas z swiatlem

      while (Serial.available())                // pentla odczytujaca wartosci po znaku odniesienia
      {
        char znak = (char)Serial.read();        // odczytywanie kolejno wartosci
        serial_odczyt2 += znak;

        swiatloCZAS = serial_odczyt2.toInt();   // konwersja odczytanej wartosci do typu int
        EEPROM.write(3,  swiatloCZAS);          //zapis wartosci czasu doswielania w EEPROM

      }

      Serial.println("          ====DANE ZAPISANE===");
      Serial.println("");

      serial_odczyt2 = "";                               //czyszczenie buforu serial monitora
  }

  if (tempOPT > 45) {                                    // kontrola poprawnoĹ›ci wprowadzonej temperatury
    Serial.print(" Wartosc nie poprawna, temperatura: ");
    Serial.print(tempOPT);
    Serial.println("'C poza telerancja!");
  }
  if (wgleby_zad > 100) {                                // kontrola poprawnosci wprowadzonej wartosci nawodnienia
    Serial.print(" Wartosc nie poprawna, nawodnienie: ");
    Serial.print(wgleby_zad);
    Serial.println("% poza telerancja!");
  }
}

///////////////////////////////////////////////////////////////////////

void informacje_startowe() {                            // funkcja wysietlajaca informacje podczas startu sterownika
  Serial.println("\n[Sterownik szklarniowy]");
  Serial.println("\n[Wersja oprogramowania v1.1]");     // serial monitor
  Serial.println("[Wykonanie Przemyslaw Murzyn 2016]");




  tft.initR(INITR_BLACKTAB);              // ustawianie czarnego tĹ‚a ekranu
  tft.fillScreen(ST7735_BLUE);            // czyszczenie ekranu
  tft.setCursor(0, 0);                    // nastawa kursora pisania po ekranie
  tft.println("Ladowanie.... ");
  tft.setTextWrap(true);
  tft.setTextColor(ST7735_RED);
  tft.println("sterownik szklarniowy_v1,0 ");
  delay(400);

  if ( f == 1) {                         // sprawdzenie czy udaĹ‚o sie poĹ‚aczyc
    tft.setTextColor(ST7735_GREEN);
    tft.println("");
    tft.println("Podlaczenie poprawne ");

    //tft.println("IP:  ", ether.myip);
    //tft.println("GW:  ", ether.gwip);    // wystompiĹ‚ problem z konwersja sprawdzic pĂłzniej
    //tft.println("DNS: ", ether.dnsip);
    Serial.println("Konfiguracja zakonczona poprawnie!");
    delay(400);
    tft.fillScreen(ST7735_BLACK);
    delay(400);
  }
}


//////////////////////////////////////////////////////////////////////////////////

void LCD(int h, int t ) {                   // funkcja wyswietlania na ekranie tft


  unsigned long time = millis();            //pobieranie aktualnego czasu dzialania programu do odswiezania ekranu

  if ( tft_odswiezTMP  < time) {
    tft_odswiezTMP = time +  tft_odswiez;   //odswiezanie ekranu co 2000ms zapobiega migotaniu ekranu

    digitalWrite(ETHERNET_CS, HIGH);        // wylaczenie komunikacji z ENC28J60
    digitalWrite(TFT_CS, LOW);              // wlaczenie komunikacji z TFT

    tft.setTextWrap(false);                                                // automatyczne zaĹ‚amywanie tekstu
    // tft.fillRoundRect( 0, 0, 128, 10, 0, tft.Color565( 0xC4, 0xC4, 0xC4) ); // elementy graficzne




    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_BLUE);
    //tft.setTextSize(2);
    tft.drawLine(0, 10, 128, 10, ST7735_GREEN);
    tft.setCursor(5, 0);
    tft.print("Dane z czujnkow:");

    tft.setTextColor(ST7735_WHITE);       //
    tft.setCursor(0, 15);                 //// wyswietlanie aktualnej temperatury w C
    tft.print("Temperatura: ");           
    tft.setTextColor(ST7735_RED);         
    tft.print(t);
    tft.print("C'");

    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(0, 25);
    tft.print("Wilgotnosc: ");              //
    tft.setTextColor(ST7735_RED);           ////wyswietlanie aktualnej wilgotnosci powietrza
    tft.print(h);                           ////
    tft.print("%");                         //

    int glebaANIM = map(wgleby, 0, 100, 0, 128);
    tft.fillRoundRect( 0, 35, glebaANIM, 10, 0, tft.Color565( 0xC4, 0xC4, 0xC4));
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor (0, 35);
    tft.print("Nawodnienie:");
    tft.setTextColor(ST7735_RED);
    tft.print (wgleby);                               // wyswietlanie aktuanej wartosci nawodniania gleby
    tft.print("%");
    //tft.setCursor (0, 35);

    if ( sterTEMP(t) == 1)                             // sprawdzanie czy wentylator pracuje
    {
      tft.setTextColor(ST7735_WHITE);                 //
      tft.setCursor (0, 45);                          //wyswietlanie stanu wentylatora
      tft.print("Wentylator:");                       //
      tft.setTextColor(ST7735_GREEN);                 //
      tft.print(" ZAL");
    }
    else                                                //// wyswietlanie stanu wentylatora
    {
      tft.setTextColor(ST7735_WHITE);                 //
      tft.setCursor (0, 45);                          //
      tft.print("Wentylator:");                       //
      tft.setTextColor(ST7735_RED);                   //
      tft.print(" WYL");                              //
    }


    if (digitalRead(swiatloON) == LOW)       // sprawdzanie czy czujnik wykrywa slonce
    {
      tft.setCursor (0, 55);
      tft.setTextColor(ST7735_MAGENTA);
      tft.print("Doswietlanie");            // wyswietlanie stanu doswietlenia. Sztuczne doswietlanie rosliny
    }
    else
    {
      tft.setCursor (0, 55);
      tft.setTextColor(ST7735_YELLOW);
      tft.print("Swiatlo dzienne");         // wyswietlnie stanu doswietlenia. Swiatlo dzienne w trakcie swiecenia lub po zakonczonym doswietlaniu


    }
    tft.drawLine(0, 63, 128, 63, ST7735_GREEN);
    tft.setCursor (0, 65);
    tft.setTextColor(ST7735_BLUE);
    tft.print("Ustawione Wartosci:");

    tft.drawLine(0, 73, 128, 73, ST7735_GREEN); //rysowanie linni ( pumkt poczatkowy, pumkt podczepienia, punkt koncowy, pukt podczepienia);

    tft.setTextColor(ST7735_WHITE);
    tft.setCursor (0, 75);
    tft.print("Temperatura:");
    tft.setTextColor(ST7735_GREEN);
    tft.print (tempOPT);                   // wyswietlanie ustawionej wartosci temperatury
    tft.print("'C");

    tft.setTextColor(ST7735_WHITE);
    tft.setCursor (0, 85);
    tft.print("Nawodnienie");
    tft.setTextColor(ST7735_GREEN);
    tft.print (wgleby_zad);                // wysietlanie ustawionej wartosci nawodnienia gleby
    tft.print("%");

    dt = clock.getDateTime();
    tft.setTextColor(ST7735_MAGENTA);
    tft.setCursor(0, 150);
    tft.print(dt.year);   tft.print("-");
    tft.print(dt.month);  tft.print("-");     //wyswietlanie czasu
    tft.print(dt.day);    tft.print(" ");
    tft.print(dt.hour);   tft.print(":");

    if (dt.minute < 10) {
      tft.print("0");                     // dodawanie zera na poczatku  minut dla dziesiatek
    }
    tft.print(dt.minute); tft.print(":");

    if (dt.second < 10) {
      tft.print("0");                    // dodawanie zera na poczatku sekund dla dziesiatek
    }
    tft.print(dt.second); tft.print("");


  }
}



//////////////////////////////////////////////////////////////////////

int sterTEMP(int t)                       //funkcjia kontroli temperatury

//int sterTEMP(int t, int tempON, int tempOFF)
{

  if (t > tempON && t > tempOFF)
  {
    digitalWrite(wiatrak, LOW);          // uruchomienie wentylatora

    return 1;

  }

  if ( t < tempOFF && t < tempON)
  {
    digitalWrite(wiatrak, HIGH);        // wylaczenie wentylatora

    return 0;

  }


}


//////////////////////////////////////////////////////////////////////

void doswietl()                                                // funkcja  doswietlania roslin po zachodzie slonca
{
  bool swiatlotmp = digitalRead(swiatlo);                    // status oswietlenia
  dt = clock.getDateTime();                                   // pobieranie aktualnego czasu

  if (swiatlotmp == HIGH && dt.hour < swiatlo_godz_wyl  )     // stan wysoki: nie swieci slonce
  {
    digitalWrite(swiatloON, LOW);                             // zapalanie lampy doswietlajacej
    swiatlo_stan = 1;
  }
  else
  {
    digitalWrite(swiatloON, HIGH);                            //gaszenie lampy doswietlajacej
    swiatlo_stan = 0;
  }
}






///////////////////////////////////////////////////////
//int w_gleby(int gleba, int wgleby , int gleba_zad)
void w_gleby()                                              // funkcja podlewania gleby
{
  int wpom;
  wpom  = analogRead(gleba);                                // pobrnie wartosci z czujnika gleby
  wgleby = map(wpom, 0, 1023, 100, 0);                      //  przenoszenie zakresu wartosci  na %

  if (wgleby < wgleby_zad) {                                // sprawdzanie czy nalezy polac

    digitalWrite(pompa, LOW);                               // urchominie pompy
    delay(5000);                                            // izolacja programu na czas podlewania
    digitalWrite(pompa, HIGH);                              // wylaczenie pompy
  }
  else {
    digitalWrite(pompa, HIGH);


  }
}


////////////////////////////////////////////////////////

void dane_wyslij(float h, float t) {               // funkcja wysyĹ‚ania danych na serwer

  digitalWrite(TFT_CS, HIGH);                      // uruchomienie komunikacji z modulem sieciowym
  digitalWrite(ETHERNET_CS, LOW);

  res = res + 1;

  ether.packetLoop(ether.packetReceive());


  if (res == 200) {                                         // okres czasu wysyĹ‚ania danych na serwer - 200 res = 10 sekund (50ms na res )

    byte sd = stash.create();
    stash.print("temperatura,");                            //
    stash.println(t);                                       //
    stash.print("wilgotnosc,");                             ////
    stash.println(h);                                       ////// uzupeĹ‚nanie pakieu danych o wartosci
    stash.print("nawodnienie,");                            ////
    stash.println(wgleby);                                  //
    stash.print("doswietlanie_zal,");                       //
    stash.println(swiatlo_stan);                            //
    stash.save();


    Stash::prepare(PSTR("PUT http://$F/v2/feeds/$F.csv HTTP/1.0" "\r\n"          //tworzenie pakieyu danych
                        "Host: $F" "\r\n"
                        "X-PachubeApiKey: $F" "\r\n"
                        "Content-Length: $D" "\r\n"
                        "\r\n"
                        "$H"),
                   website, PSTR(FEED), website, PSTR(APIKEY), stash.size(), sd);


    session = ether.tcpSend();                                                  // wysylanie pakietu
  }

  const char* reply = ether.tcpReply(session);

  if (reply != 0) {                                                              // sprwdzenie czy serwer odpowiedzia
    res = 0;                                                                     // reet licznika czasu
    Serial.println("Polaczenie OK");
    Serial.println(" ");
  }


  if (res > 220) {                                                               // wykrywanie braku odpowiedzi w celu uzuskania ponownego polaczenia
    Serial.println("sprawdz polacznie sieciowe");
    Ethernet_uruchom();

  }

  delay(50);

  ////////////////////////////////////////////////
}


void Ethernet_uruchom ()
{


  int ether_reset = 14;
  pinMode(ether_reset, OUTPUT);

  Serial.println("Proba podlaczenia...");
  Serial.println(" ");
  digitalWrite(ether_reset, LOW);
  delay(1000);                                                                       //Restart moduĹ‚u ethernetowego
  digitalWrite(ether_reset, HIGH);
  delay(500);


  if (ether.begin(sizeof Ethernet::buffer, mymac, ETHERNET_CS) == 0) {               // ustawianie karty sieciowej
    Serial.println( "Failed to access Ethernet controller");
    f = 0 ;
  }

  if (!ether.dhcpSetup()) {                                                         // konfiguracja i sprawdzanie statusu DHCP
    Serial.println("Problem z DHCP");
    f = 0 ;
    // break;
  }



  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);                                               // wyswietlanie na serial przydzielonych adresow
  ether.printIp("DNS: ", ether.dnsip);
  f = 1 ;

  if (!ether.dnsLookup(website))                                                    // konfiguracja i sprawdzanie dns
    Serial.println("DNS problem");

  ether.printIp("SRV: ", ether.hisip);                                              // wyswietlanie na serial adresu podlaczonego serwera
  Serial.println(" ");


  res = 0;                                                                           // reset licznika  celu kontynuacji programu
}


/////////////////////////////////////////////////////////////////////////////


void przerwanie() {                                                                  // obsluga przerwania

  swiatlo_timeH = dt.minute;                                                        // zapisywanie momentu wschodu slonca
  swiatlo_timeM = dt.minute;                                                        //
  Serial.print("Slonce wstalo o godzinie: ");
  Serial.print( dt.hour); Serial.print(":");
  if (swiatlo_timeM < 10) {
    Serial.print("0");                                                               // dodawanie zera na poczatku  minut dla dziesiatek
  }

  Serial.println(swiatlo_timeM);

  swiatlo_godz_wyl =  dt.hour + swiatloCZAS;                                        // ustaleni nowej wartosci wylaczenia swiatla
  EEPROM.write(4, swiatlo_godz_wyl);
  Serial.print("Godzina wylaczenia swiatla:");
  Serial.print(swiatlo_godz_wyl); Serial.print(":");


  if (swiatlo_timeM < 10) {
    Serial.print("0");                                                               // dodawanie zera na poczatku  minut dla dziesiatek
  }

  Serial.println(swiatlo_timeM);

}




