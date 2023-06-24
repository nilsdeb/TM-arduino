

///////////////////////////////   structure   /////////////////////////////////////////








///////////////////////////////   librairie   /////////////////////////////////////////


// librairie de IMU
#include <Arduino_LSM6DS3.h>

// librairie gps
#include <TinyGPS++.h>

// libraire de com avec les capteurs
#include <SoftwareSerial.h>

// librairie pour ecrire sur la carte sd
#include <SD.h>


///////////////////////////////   definition   /////////////////////////////////////////


// defini pin de la carte SD (a changer)
const int chipSelect = 10;

// defini pin bouton (a changer)
const int boutonPin = 2;

//defini pin GPS RX = reception    TX = transmission
static const int RXPin = 3, TXPin = 4;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// defini communication avec GPS
static const uint32_t GPSBaud = 9600;

// Variable si ca mesure ou pas
bool mesureEnCours = false;

// The TinyGPS++ object
TinyGPSPlus gps;

// Objet de fichier pour la carte SD
File dataFile;





void setup() {

  // moniteur
  Serial.begin(9600);

  // allume IMU
  IMU.begin();

  //gps
  ss.begin(GPSBaud);

  // Configure le bouton en entrée
  pinMode(boutonPin, INPUT);

  
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
