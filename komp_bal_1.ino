//karta SD
#include <SPI.h>               
#include <SD.h>  

Sd2Card card;
File plik;

//RTC
#include <Wire.h>
#include <DS3231.h>

DS3231 clock;
RTCDateTime dt;

// TEMP CZUJNIK
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMP 10  //zdefiniowanie pinu temp

OneWire oneWire(TEMP); //podłączony do pinu TEMP
DallasTemperature sensors(&oneWire); //Przekazania informacji do biblioteki
float temp=0;

//CIŚNIENIE, WYSOKOŚĆ, temperatura
#include <LPS331.h>

LPS331 ps;
float pressure = 0;
float altitude = 0;
float temperature = 0;


void setup() {
  Serial.begin(9600);

  clock.begin(); //inicjalizacja RTC
  sensors.begin(); //Inicjalizacja czujnikow temp

  clock.setDateTime(__DATE__, __TIME__); //ustawienie formatu RTC

  //sprawdzenie karty SD
  while (!Serial) {
    ; //Wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("Initializing SD card...");
  if(SD.begin(10)){
    Serial.println("Błąd inicjalizacji karty SD");
    return;
  }
  Serial.println("Karta jest gotowa do zapisu..."); 
  }

  //spradzenie czujnika ciśnienia
  Wire.begin(); //inicjalizacja czujnika ciśnienia
  if (!ps.init())
  {
    Serial.println("Błąd inicjalizacji czujnika ciśnienia");
    while (1);
  }
  ps.enableDefault();
  
  
}

void loop() {
  //RTC
  dt = clock.getDateTime(); //odczyt daty

  //TEMP
  sensors.requestTemperatures(); //pobranie danych z czujnika temp
  temp = sensors.getTempCbyIndex(0); //odczytanie temp z czujnika 0

  //CIŚNIENIE, WYSOKOŚĆ
  pressure = ps.readPressureMillibars();
  altitude = ps.pressureToAltitudeMeters(pressure);
  temperature = ps.readTemperatureC();

  File plik = SD.open("dane.txt", FILE_WRITE);
  if (plik){
      plik.println(dt); \\data
      
      plik.print("Temp z czujnika: ");  \\temp z czujnika
      plik.print(temp);
      plik.println(" C")
      
      plik.print("Cisnienie: "}; \\ciśnienie, wysokość, temperatura
      plik.print(pressure);
      plik.println(" mbar\ta: ");
      plik.print("Wysokość: "};
      plik.print(altitude);
      plik.println(" m\tt");
      plik.print("Temperatura: "};
      plik.print(temperature);
      plik.println(" C");
     }
   plik.close();

   delay(1000);
}
