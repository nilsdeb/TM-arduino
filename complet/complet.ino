

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
bool mesure = false;

// The TinyGPS++ object
TinyGPSPlus gps;

// Objet de fichier pour la carte SD
File dataFile;

// structure qui nous permet de de print lireMesure
struct Mesure {
  float accX;
  float accY;
  float accZ;
  float gyroX;
  float gyroY;
  float gyroZ;
  float latitude;
  float longitude;
};


void setup() {

  // met en place la communication avec le moniteur, que utilie avec l ordi
  Serial.begin(9600);

  // allume IMU
  IMU.begin();

  // gps
  ss.begin(GPSBaud);

  // Configure le bouton en entrée
  pinMode(boutonPin, INPUT);

  // allume la carte sd
  if (!SD.begin(chipSelectPin)) {

    // si elle s allume pas attendre 50 ms
    delay(50);

    // reessayer
    SD.begin(chipSelectPin);
  }
  // ouverture du fichier
  dataFile = SD.open("donnees.txt", FILE_WRITE);

  // Vérification si le fichier est ouvert avec succès
  if (!dataFile) {
    Serial.println("erreur d'ouverture");
    while (1);
  }
}


void loop() {

  // verifie l etat du bouton
  if (digitalRead(boutonPin) == HIGH) {

    // attend 50 ms
    delay(50);

    // verifie si le bouton est toujours allume
    if (digitalRead(boutonPin) == HIGH) {

      // verifie la constente mesure  ! inverse la logique, donc demare quand c est faux
      if (!mesure) {

        // change l etat de la variable
        mesure = true;
        
        // Ouverture du fichier pour écriture en ajoutant les données
        dataFile = SD.open("donnees.txt", FILE_WRITE);

          // controle de l'ouverture
          while (!dataFile) {
            
          Serial.println("Erreur lors de l'ouverture du fichier !");
          
          delay(1000);

          // Réessayer l'ouverture du fichier
          dataFile = SD.open("donnees.txt", FILE_WRITE); 
          }
        } 
      } else {
      
      // change l etat de la variable
      mesure = false;

      // ferme le fichier
      dataFile.close();    
      }
  }

  // verifie si la constante
  if (mesure) {

    // precise ce qu est mesure
    float mesure = lireMesure();

    // comprendre de quoi on parle
    Serial.print("Mesure : ");

    // imprime sur le serial
    Serial.println(mesure);

    // Écrire la mesure dans le fichier sur la carte SD
    dataFile.println(mesure);  

  } else {
}
     
// code de lecture de l imu et du gps
float lireMesure() {
  
  // declaration d'un objet interne a la bibli
  sensors_event_t accelEvent;

  // capture des donnees
  IMU.readAcceleration(accelEvent);
  mesure.accX = accelEvent.acceleration.x;
  mesure.accY = accelEvent.acceleration.y;
  mesure.accZ = accelEvent.acceleration.z;
  
  // declaration d'un objet interne a la bibli
  sensors_event_t gyroEvent;
  
  // capture des donnees
  IMU.readGyro(gyroEvent);
  mesure.gyroX = gyroEvent.gyro.x;
  mesure.gyroY = gyroEvent.gyro.y;
  mesure.gyroZ = gyroEvent.gyro.z;

  // test du signal gps
  TinyGPSPlus gps;
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  // lecture gps
  mesure.latitude = gps.location.lat();
  mesure.longitude = gps.location.lng();

  // mets tout cela en forma mesure
  return mesure;
}

}
