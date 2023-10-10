#include <Arduino_LSM6DS3.h>    //librairie de IMU

void setup() {
  
  Serial.begin(9600);   //moniteur

  IMU.begin();    //allume IMU

}

void loop() {
  float x, y, z;
  IMU.readAcceleration(x, y, z);    //precise x,y,z sont de l accelerometre
    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  IMU.readGyroscope(x, y, z);
    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
    Serial.println();
  delay(3000);   //attend 3 secondes (en mili sec.)
}
