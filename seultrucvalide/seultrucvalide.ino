#include <Arduino_LSM6DS3.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

static const int rxPin = 10, txPin = 11;
static const uint32_t GPSBaud = 4800;

// Créer une instance de la classe SoftwareSerial pour communiquer avec le GPS
SoftwareSerial gpsSerial(rxPin, txPin);


// The TinyGPSPlus object
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  IMU.begin();
  gpsSerial.begin(9600);
}

void loop() {

    
  
    float accelX, accelY, accelZ;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accelX, accelY, accelZ);
    
    // Print accelerometer data
    Serial.print(accelX);
    Serial.println();
    Serial.print(accelY);
    Serial.println();
    Serial.print(accelZ);
    Serial.println();
  }

  // Read gyroscope data
  float gyroX, gyroY, gyroZ;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gyroX, gyroY, gyroZ);
    
    // Print gyroscope data
    Serial.println();
    Serial.print(gyroX);
    Serial.println();
    Serial.print(gyroY);
    Serial.println();
    Serial.print(gyroZ);
    Serial.println();
  }
    // Vérifier si des données sont disponibles depuis le GPS
  if (gpsSerial.available() > 0) {
    // Lire les données du GPS
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
    
    // Vérifier si des données valides ont été reçues du GPS
    if (gps.location.isValid()) {
      Serial.println();
      Serial.println(gps.location.lat(), 6);
      Serial.println();
      Serial.println(gps.location.lng(), 6);
    } else {
    Serial.println();
    Serial.println(0);
    Serial.println();
    Serial.println(0);
    }
  } else {
    Serial.println();
    Serial.println(0);
    Serial.println();
    Serial.println(0);
    }
  delay(500);
}
