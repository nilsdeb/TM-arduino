
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
const int boutonPin = 5;

//defini pin GPS RX = reception    TX = transmission
static const int RXPin = 3, TXPin = 4;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// defini communication avec GPS
static const uint32_t GPSBaud = 9600;

// structure qui nous permet de stocker les mesures
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

//varible si il y a ou non mesure
bool mesureEnCours = false;

//fichier creer dans la carte sd
File dataFile;


///////////////////////////////   lire mesure   /////////////////////////////////////////



void lireMesure(Mesure &mesure) {

  // creez une instance de TinyGPSPlus
    TinyGPSPlus gps;

  //stockage temporaire des donnees
  float accelX, accelY, accelZ;
  float gyroX, gyroY, gyroZ;
  
  // lire les donnees
  IMU.readAcceleration(accelX, accelY, accelZ);
  IMU.readGyroscope(gyroX, gyroY, gyroZ);
  
  //  mettre a jour la structure mesure
  mesure.accX = accelX;
  mesure.accY = accelY;
  mesure.accZ = accelZ;
  mesure.gyroX = gyroX;
  mesure.gyroY = gyroY;
  mesure.gyroZ = gyroZ;

  // verifie l etat du signal gps et le lis
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  //  mettre a jour la structure mesure
  mesure.latitude = gps.location.lat();
  mesure.longitude = gps.location.lng();
}


///////////////////////////////   setup   /////////////////////////////////////////


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


///////////////////////////////   loop   /////////////////////////////////////////


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
  //verifie la variable mesure
  if (mesureEnCours) {

    //declare une variable type mesure
    Mesure mesure;

    //appele la fonction lire mesure
    lireMesure(mesure);

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
    dataFile.println(mesure.longitude);
  }     
}
