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
  float x, y, z;
  
  Serial.print("Accelerometre");    //que on comprenne quelle chiffre est quoi
  Serial.println();   //laisse une ligne vide
  
  IMU.readAcceleration(x, y, z);    //precise x,y,z sont de l accelerometre
    Serial.print(x);
    Serial.print('\t');   //laisse de l espace dans l affichage
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);

  Serial.print("Gyroscope");
  Serial.println();
  
  IMU.readGyroscope(x, y, z);
    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  delay(300);   //attend 3 secondes (en mili sec.)
}
