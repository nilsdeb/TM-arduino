#include <Arduino_LSM6DS3.h>    //librairie de IMU
#include <TinyGPS++.h>    //librairie gps
#include <SoftwareSerial.h>   //libraire de comme avec capteur externe

static const int RXPin = 3, TXPin = 4;    //defini pin GPS
static const uint32_t GPSBaud = 9600;   // defini communication avec GPS


TinyGPSPlus gps;    // The TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin);    // The serial connection to the GPS device


void setup() {
  
  Serial.begin(9600);   //moniteur
  IMU.begin();    //allume IMU
  ss.begin(GPSBaud);    //gps

  Serial.println("Acceleration et gyroscope interne up");
  Serial.println();
}


void loop() {
  
 while (ss.available() > 0)    // This sketch displays information every time a new sentence is correctly encoded.
    if (gps.encode(ss.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10){
    Serial.println(F("No GPS detected: check wiring. Wait for all data"));
    while(true);
  }
  delay(300);   //attend 0.3 secondes (en mili sec.)
}



void displayInfo() {
  float x, y, z;
  
  Serial.print("Accelerometre:  ");    //que on comprenne quelle chiffre est quoi
  Serial.println();   //laisse une ligne vide
  
  IMU.readAcceleration(x, y, z);    //precise x,y,z sont de l accelerometre
    Serial.print(x);
    Serial.print('\t');   //laisse de l espace dans l affichage
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);

  Serial.print("Gyroscope:  ");
  Serial.println();
  
  IMU.readGyroscope(x, y, z);
    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);

  Serial.print("Location: "); 
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.println(z);

    
  Serial.print("Time: ");
  if (gps.date.isValid()){
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }

  Serial.print("  ");
  if (gps.time.isValid()){
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  
  Serial.println();
}
