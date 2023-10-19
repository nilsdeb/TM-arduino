#include <Arduino_LSM6DS3.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// Définir les broches RX et TX pour la communication avec le capteur GPS
const int rxPin = 10;  // Broche de réception (RX) du GPS connectée à la broche 2 de l'Arduino
const int txPin = 11;  // Broche de transmission (TX) du GPS connectée à la broche 3 de l'Arduino

// Créer une instance de la classe SoftwareSerial pour communiquer avec le GPS
SoftwareSerial gpsSerial(rxPin, txPin);

// Créer une instance de la classe TinyGPSPlus
TinyGPSPlus gps;

void setup() {
  // Initialisation de la communication série avec une vitesse de 9600 bauds
  Serial.begin(9600);

  // Initialisation de la communication série avec le GPS à la même vitesse
  gpsSerial.begin(9600);

  IMU.begin();
}

void loop() {
  float accelX, accelY, accelZ;
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(accelX, accelY, accelZ);
      // Print accelerometer data
      Serial.print("ax=");
      Serial.print(accelX);
      Serial.print(", ay=");
      Serial.print(accelY);
      Serial.print(", az=");
      Serial.print(accelZ);
     }
  float gyroX, gyroY, gyroZ;
      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gyroX, gyroY, gyroZ);
        // Print gyroscope data
        Serial.print(", gx=");
        Serial.print(gyroX);
        Serial.print(", gy=");
        Serial.print(gyroY);
        Serial.print(", gz=");
        Serial.print(gyroZ);
      }
  // Vérifier si des données sont disponibles depuis le GPS
  if (gpsSerial.available() > 0) {
    // Lire les données du GPS
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
    
    // Vérifier si des données valides ont été reçues du GPS
    if (gps.location.isValid()) {
      Serial.print(", lat=");
      Serial.print(gps.location.lat(), 6);
      Serial.print(", lon=");
      Serial.print(gps.location.lng(), 6);
    } else {
      Serial.print(", lat=");
      Serial.print(0);
      Serial.print(", lon=");
      Serial.print(0);
    }

  }else {
    Serial.print(", lat=");
    Serial.print(0);
    Serial.print(", lon=");
    Serial.print(0);
  }
  
  Serial.println();
  delay(500);
}
