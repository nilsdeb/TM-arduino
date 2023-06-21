#include <Arduino_LSM6DS3.h>    //librairie de IMU
#include <TinyGPS++.h>    //librairie gps
#include <SoftwareSerial.h>   //libraire de comme avec capteur externe

static const int RXPin = 3, TXPin = 4;    //defini pin GPS
static const uint32_t GPSBaud = 9600;   // defini communication avec GPS


const int boutonPin = 2; // Broche utilisée pour le bouton a changer
bool mesureEnCours = false; // Variable pour suivre l'état de la mesure





void setup() {
  
  Serial.begin(9600);   //moniteur
  IMU.begin();    //allume IMU
  ss.begin(GPSBaud);    //gps
  
  pinMode(boutonPin, INPUT);  // Configure le bouton en entrée
  TinyGPSPlus gps;    // The TinyGPS++ object
  SoftwareSerial ss(RXPin, TXPin);    // The serial connection to the GPS device
  Serial.println("arduino up");
  Serial.println();
}




void loop() {
  if (digitalRead(boutonPin) == HIGH) { // Vérifie l'état du bouton
    delay(50); // Attente pour éviter les rebonds du bouton
    
    if (digitalRead(boutonPin) == HIGH) { // Vérifie si le bouton est toujours enfoncé
      if (!mesureEnCours) {
        demarrerMesure(); // Démarre la mesure si elle n'est pas déjà en cours
      } else {
        arreterMesure(); // Arrête la mesure si elle est en cours
      }
    }
  }
}

void demarrerMesure() {
  mesureEnCours = true;   //change la variable pour savoir si la mesure est en cours ou non
  while (ss.available() > 0)    // This sketch displays information every time a new sentence is correctly encoded.
    if (gps.encode(ss.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10){
    Serial.println(F("No GPS detected: check wiring. Wait for all data"));
    while(true);
  }
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
    Serial.print(F("."));
    Serial.print(gps.date.day());
    Serial.print(F("."));
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
  delay(25);
}






void arreterMesure() {
  mesureEnCours = false;
  
  // Ajoutez ici le code pour arrêter votre mesure
}








// reste a faire
// structurere et rendre lisible , refaire cette liste de tache au propore
//verifier se l'histoire du bouton fonctionne
//inclure la question de l eccriture des donnée sur la cartes sd
//inclure les donnée de vitesse donnée par le gps
//code sd et gps par chat en dessous




#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (10)

Adafruit_BNO055 bno = Adafruit_BNO055();

File dataFile; // Fichier pour enregistrer les données sur la carte SD

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }

  if (!SD.begin(10)) {
    Serial.println("Échec de l'initialisation de la carte SD !");
    return;
  }

  if (!bno.begin()) {
    Serial.println("Échec de l'initialisation du BNO055 !");
    while (1);
  }

  dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Timestamp, Accel X, Accel Y, Accel Z, Gyro X, Gyro Y, Gyro Z, Euler Heading, Euler Roll, Euler Pitch");
    dataFile.close();
  } else {
    Serial.println("Erreur lors de l'ouverture du fichier !");
  }
}

void loop() {
  sensors_event_t event;
  bno.getEvent(&event);

  dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(millis());
    dataFile.print(", ");
    dataFile.print(event.acceleration.x);
    dataFile.print(", ");
    dataFile.print(event.acceleration.y);
    dataFile.print(", ");
    dataFile.print(event.acceleration.z);
    dataFile.print(", ");
    dataFile.print(event.gyro.x);
    dataFile.print(", ");
    dataFile.print(event.gyro.y);
    dataFile.print(", ");
    dataFile.print(event.gyro.z);
    dataFile.print(", ");
    dataFile.print(event.orientation.x);
    dataFile.print(", ");
    dataFile.print(event.orientation.y);
    dataFile.print(", ");
    dataFile.println(event.orientation.z);

    dataFile.close();
  } else {
    Serial.println("Erreur lors de l'ouverture du fichier !");
  }

  delay(BNO055_SAMPLERATE_DELAY_MS);
}





#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(10, 11); // Définir les broches RX et TX du module GPS

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
  if (gpsSerial.available()) {
    String gpsData = gpsSerial.readStringUntil('\n');
    if (gpsData.startsWith("$GPRMC")) {
      // Récupérer les données de vitesse
      char speedData[7];
      int index = 0;
      for (int i = 0; i < gpsData.length(); i++) {
        if (gpsData[i] == ',') {
          index++;
          if (index == 7) {
            i++;
            for (int j = 0; j < 6; j++) {
              speedData[j] = gpsData[i++];
            }
            speedData[6] = '\0';
            break;
          }
        }
      }

      // Convertir la vitesse en float
      float speed = atof(speedData);

      // Afficher la vitesse
      Serial.print("Vitesse : ");
      Serial.print(speed);
      Serial.println(" km/h");
    }
  }
}
