//pin connection
//Arudino I2C interface
//SDC->aruidno nano A5 with 10kohm pull-up reistor
//SDA->aruidno nano A4 with 10kohm pull-up reistor
//MKI 209 documentation link https://www.st.com/resource/en/datasheet/iis2iclx.pdf
//2D inclinometer
//not FIFO output

#include <Wire.h>
byte accelX, accelY,temp;
float gForceX, gForceY,temperature;

void setup() {
  Serial.begin(2000000);
  Wire.begin();
  setupData();

}
void loop() {
  recordAccelRegisters();
  readtemperature();
  processData();
  printData();
  delay(100);
}
void setupData() {
  //Slave ADdress (SAD) last digit depends on SDO/SA0 voltage level 
  //confige device
  //01h
  Wire.beginTransmission(0b1101011); //SDO/SA0 is High level
  Wire.write(0x01);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(10);
  
  //02h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x02);
  Wire.write(0x7F);
  Wire.endTransmission();
  delay(10);

  //07h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x07);
  Wire.write(0b00000100);
  Wire.endTransmission();
  delay(10);
  
  //10h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x10);
  Wire.write(0b00011100);
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
  Wire.write(0b00000000);
  Wire.endTransmission();
  delay(10);
  
  //14h
  Wire.beginTransmission(0b1101011);
  Wire.write(0x14);
  Wire.write(0b00000000);
  Wire.endTransmission();
  delay(10);

}
void recordAccelRegisters() {
  //gyro data from 22h to 2Bh
  Wire.beginTransmission(0b1101011);
  Wire.write(0x22);
  Wire.endTransmission();
  //read 6 bytes data back
  Wire.requestFrom(0b1101011, 4);
  
  //2's complement conversion
  while (Wire.available() < 6);
  gyroX = ~(Wire.read() | Wire.read() << 8) + 1;
  gyroY = ~(Wire.read() | Wire.read() << 8) + 1;
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

void processData() {
  gForceX = accelX * 0.061; //for ±2g 0.061mg/LSB
  gForceY = accelY * 0.061;
  temperature=temp/256+25; // 256LSB/degree
}
void  printData() {

  Serial.print("X(mg):");
  Serial.print(accelX);
  Serial.print("\t");
  
  Serial.print("Y(mg):");
  Serial.print(accelY);
  Serial.print("\t");
  
  Serial.print("temp");
  Serial.println(temperature);
}
