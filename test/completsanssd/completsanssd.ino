///////////////////////////////   librairie   /////////////////////////////////////////


// librairie de IMU
#include <Arduino_LSM6DS3.h>

// librairie gps
#include <TinyGPS++.h>

// libraire de com avec les capteurs
#include <SoftwareSerial.h>


///////////////////////////////   definition   /////////////////////////////////////////


// defini pin bouton (a changer)
const int boutonPin = 8;

//defini pin GPS RX = reception    TX = transmission
static const int RXPin = 4, TXPin = 3;

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
  mesure.latitude = gps.location.lat(), 6;
  mesure.longitude = gps.location.lng(), 6;
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

}


///////////////////////////////   loop   /////////////////////////////////////////


void loop() {

  // verifie l etat du bouton
  if (digitalRead(boutonPin) == HIGH) {

    // attend 100 ms
    delay(100);

    // verifie si le bouton est toujours allume
    if (digitalRead(boutonPin) == HIGH) {

      // verifie la constente mesure  ! inverse la logique, donc demare quand c est faux
      if (!mesureEnCours) {

        // change l etat de la variable
        mesureEnCours = true;
        
        } 
      } else {
      
      // change l etat de la variable
      mesureEnCours = false; 
      }
    // le mettre sinon le timing pour apuier sur le bouton devient sport!
    delay(100);
  }
  //verifie la variable mesure
  if (mesureEnCours) {

    //declare une variable type mesure
    Mesure mesure;

    delay(1000);
    
    //appele la fonction lire mesure
    lireMesure(mesure);

    Serial.println("accelerometre :");
    Serial.println(mesure.accX);
    Serial.println(mesure.accY);
    Serial.println(mesure.accZ);
    Serial.println("gyroscope :");
    Serial.println(mesure.gyroX);
    Serial.println(mesure.gyroY);
    Serial.println(mesure.gyroZ);
    Serial.println("gps :");
    Serial.println(mesure.latitude);
    Serial.println(mesure.longitude);

    
  }
}
