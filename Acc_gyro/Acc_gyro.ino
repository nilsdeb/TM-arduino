#include <Arduino_LSM6DS3.h>    //librairie de IMU

void setup() {
  
  Serial.begin(9600);   //moniteur

  IMU.begin();    //allume IMU

  Serial.println("Acceleration in g's");
  Serial.println("X\tY\tZ");
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
