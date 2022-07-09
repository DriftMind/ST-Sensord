//pin connection
//Arudino I2C interface
//SDC->aruidno nano A5 with 10kohm pull-up reistor
//SDA->aruidno nano A4 with 10kohm pull-up reistor
//MKI 194 documentation link https://www.st.com/resource/en/datasheet/lsm6dsr.pdf
//MKI 207 documentation link https://www.st.com/resource/en/datasheet/ism330dhcx.pdf
//3D accelerometer and 3D gyroscope
//not FIFO output

#include <Wire.h>
byte accelX, accelY, accelZ,temp;
float gForceX, gForceY, gForceZ,temperature;

byte gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;
void setup() {
  Serial.begin(2000000);
  Wire.begin();
  setupData();

}
void loop() {
  recordAccelRegisters();
  recordGyroRegisters();
  readtemperature();
  processData();
  printData();
  delay(100);
}
void setupData() {
  //Slave ADdress (SAD) last digit depends on SDO/SA0 voltage level 
  //confige device
  //10h
  Wire.beginTransmission(0b1101011); //SDO/SA0 is High level
  Wire.write(0x10);
  Wire.write(0b00010000);
  Wire.endTransmission();
  delay(10);
  
  //11h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x11);
  Wire.write(0b10100100);
  Wire.endTransmission();
  delay(10);

  //12h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x12);
  Wire.write(0b00000100);
  Wire.endTransmission();
  delay(10);
  
  //13h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x13);
  Wire.write(0b00000010);
  Wire.endTransmission();
  delay(10);
  
  //14h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x14);
  Wire.write(0b01100000);
  Wire.endTransmission();
  delay(10);
  
  //15h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x15);
  Wire.write(0b10000000);
  Wire.endTransmission();
  delay(10);
  
  //16h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x16);
  Wire.write(0b00000000);
  Wire.endTransmission();

}
void recordGyroRegisters() {
  //gyro data from 22h to 27h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x22);
  Wire.endTransmission();
  //read 6 bytes data back
  Wire.requestFrom(0b1101011, 6);
  
  //2's complement conversion
  while (Wire.available() < 6);
  gyroX = ~(Wire.read() | Wire.read() << 8) + 1;
  gyroY = ~(Wire.read() | Wire.read() << 8) + 1;
  gyroZ = ~(Wire.read() | Wire.read() << 8) + 1;
}
void readtemperature() {
  //temperature data from 20h to 21h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x20);
  Wire.endTransmission();
    //read 2 bytes data back
  Wire.requestFrom(0b1101011, 2);
  while (Wire.available() < 2);
  //2's complement conversion
  temp = ~(Wire.read() | Wire.read() << 8) + 1;
}
void recordAccelRegisters() {
  
  //acceleration from 28h to 2Dh
  Wire.beginTransmission(0b1101011);
  Wire.write(0x28);
  Wire.endTransmission();
  //read 6 bytes data back
  Wire.requestFrom(0b1101011, 6);
  
  while (Wire.available() < 6);
  //2's complement conversion
  accelX = ~(Wire.read() | Wire.read() << 8) + 1;
  accelY = ~(Wire.read() | Wire.read() << 8) + 1;
  accelZ = ~(Wire.read() | Wire.read() << 8) + 1;


}
void processData() {
  gForceX = accelX * 0.061;
  gForceY = accelY * 0.061;
  gForceZ = accelZ * 0.061;
  rotX = gyroX *4.375;
  rotY = gyroY *4.375;
  rotZ = gyroZ *4.375;
  temperature=temp/256+25;
}
void  printData() {

  Serial.print("X(mg):");
  Serial.print(accelX);
  Serial.print("\t");
  
  Serial.print("Y(mg):");
  Serial.print(accelY);
  Serial.print("\t");
  
  Serial.print("Z(mg):");
  Serial.print(accelZ);
  Serial.print("\t");
  
  Serial.print("X(mdps):");
  Serial.print(rotX);
  Serial.print("\t");
  
  Serial.print("Y(mdps):");
  Serial.print(rotY);
  Serial.print("\t");
  
  Serial.print("Z(mdps):");
  Serial.print(rotZ);
  Serial.print("\t");
  
  Serial.print("temp");
  Serial.println(temperature);
}
