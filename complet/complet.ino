

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



// code de lecture de l imu et du gps
void lireMesure() {
  float accelX, accelY, accelZ;
  float gyroX, gyroY, gyroZ;

  

   Mesure mesure;


  IMU.readAcceleration(accelX, accelY, accelZ);
  IMU.readGyroscope(gyroX, gyroY, gyroZ);

  while (ss.available() > 0) {
    gps.encode(ss.read());
  }
  
  float latitude = gps.location.lat();
  float longitude = gps.location.lng();

  // Mettre à jour la structure Mesure
  mesure.accX = accelX;
  mesure.accY = accelY;
  mesure.accZ = accelZ;
  mesure.gyroX = gyroX;
  mesure.gyroY = gyroY;
  mesure.gyroZ = gyroZ;
  mesure.latitude = latitude;
  mesure.longitude = longitude;
}

  // mets tout cela en forma mesure
  //return mesure;
}

// Variable si ca mesure ou pas
bool mesureEnCours = false;

// The TinyGPS++ object
TinyGPSPlus gps;

// Objet de fichier pour la carte SD
File dataFile;



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
  if (!SD.begin(chipSelect)) {

    // si elle s allume pas attendre 50 ms
    delay(50);

    // reessayer
    SD.begin(chipSelect);
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
      if (!mesureEnCours) {

        // change l etat de la variable
        mesureEnCours = true;
        
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
      mesureEnCours = false;

      // ferme le fichier
      dataFile.close();    
      }
  }

  // verifie si la constante
  if (mesure) {

    // precise ce qu est mesure
    float mesure = lireMesure();

    dataFile.print("AccX=");
    dataFile.print(mesure.accX);
    dataFile.print(", AccY=");
    dataFile.print(mesure.accY);
    dataFile.print(", AccZ=");
    dataFile.print(mesure.accZ);
    dataFile.print(", GyroX=");
    dataFile.print(mesure.gyroX);
    dataFile.print(", GyroY=");
    dataFile.print(mesure.gyroY);
    dataFile.print(", GyroZ=");
    dataFile.print(mesure.gyroZ);
    dataFile.print(", Latitude=");
    dataFile.print(mesure.latitude);
    dataFile.print(", Longitude=");
    dataFile.println(mesure.longitude)
  }     
}
