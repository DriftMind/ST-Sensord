#include <Wire.h>
long accelX, accelY, accelZ,temp;
float gForceX, gForceY, gForceZ,temperature;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;
void setup() {
  Serial.begin(2000000);
  Wire.begin();
  //For non-207
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

  Wire.beginTransmission(0b1101011);
  Wire.write(0x10);
  Wire.write(0b00010000);
  Wire.endTransmission();
  delay(10);

  Wire.beginTransmission(0b1101011);
  Wire.write(0x11);
  Wire.write(0b10100100);
  Wire.endTransmission();
  delay(10);

  Wire.beginTransmission(0b1101011);
  Wire.write(0x12);
  Wire.write(0b00000100);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(0b1101011);
  Wire.write(0x13);
  Wire.write(0b00000010);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(0b1101011);
  Wire.write(0x14);
  Wire.write(0b01100000);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(0b1101011);
  Wire.write(0x15);
  Wire.write(0b10000000);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(0b1101011);
  Wire.write(0x16);
  Wire.write(0b00000000);
  Wire.endTransmission();

}
void recordGyroRegisters() {
  Wire.beginTransmission(0b1101011);
  Wire.write(0x22);
  Wire.endTransmission();
  Wire.requestFrom(0b1101011, 6);
  while (Wire.available() < 6);
  gyroX = ~(Wire.read() | Wire.read() << 8) + 1;
  gyroY = ~(Wire.read() | Wire.read() << 8) + 1;
  gyroZ = ~(Wire.read() | Wire.read() << 8) + 1;
}
void readtemperature() {
  Wire.beginTransmission(0b1101011);
  Wire.write(0x20);
  Wire.endTransmission();
  Wire.requestFrom(0b1101011, 2);
  while (Wire.available() < 2);
  temp = ~(Wire.read() | Wire.read() << 8) + 1;
}
void recordAccelRegisters() {
  Wire.beginTransmission(0b1101011);
  Wire.write(0x28);
  Wire.endTransmission();

  //For other
  Wire.requestFrom(0b1101011, 6);
  while (Wire.available() < 6);
  accelX = ~(Wire.read() | Wire.read() << 8) + 1;
  accelY = ~(Wire.read() | Wire.read() << 8) + 1;
  accelZ = ~(Wire.read() | Wire.read() << 8) + 1;


}
void processData() {
  gForceX = accelX * 0.061;
  gForceY = accelY * 0.061;
  gForceZ = accelZ * 0.061;
  rotX = gyroX *0.004375;
  rotY = gyroY *0.004375;
  rotZ = gyroZ *0.004375;
  temperature=temp/256+25;
}
void  printData() {

 Serial.print("X(g):");
  Serial.print(accelX);


  Serial.print("\t");
  Serial.print("Y(g):");
  Serial.print(accelY);

  Serial.print("\t");
  Serial.print("Z(g):");
  Serial.print(accelZ);

  Serial.print("\t");
  Serial.print("X(dps):");
  Serial.print(rotX);

  Serial.print("\t");
  Serial.print("Y(dps):");
  Serial.print(rotY);

  Serial.print("\t");
  Serial.print("Z(dps):");
  Serial.print(rotZ);

  Serial.print("\t");
  Serial.print("temp");
  Serial.println(temperature);




}
