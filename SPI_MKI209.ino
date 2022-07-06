#include <SPI.h>
const int CS=10;
byte accelX0, accelY0, accelZ0,tempL,tempH;
byte accelX1, accelY1, accelZ1;
double gForceX,gForceY,gForceZ, temperature;
byte who;

void setup() {
  Serial.begin(9600);
  pinMode(CS,OUTPUT); 
  //SPI.setBitOrder(MSBFIRST);
  //SPI.setDataMode(SPI_MODE3);
  
  SPI.beginTransaction(SPISettings(10000000,MSBFIRST,SPI_MODE3));
  SPI.endTransaction();
  SPI.begin();
  setupData();
}

void setupData(){
  
  //config device only SPI
  digitalWrite(CS,LOW);
  SPI.transfer(0x01);
  SPI.transfer(0x00);
  digitalWrite(CS,HIGH);

  digitalWrite(CS,LOW);
  SPI.transfer(0x01);
  SPI.transfer(0x7F);
  digitalWrite(CS,HIGH);

  digitalWrite(CS,LOW);
  SPI.transfer(0x10);
  SPI.transfer(0x1C);
  digitalWrite(CS,HIGH);

  digitalWrite(CS,LOW);
  SPI.transfer(0x12);
  SPI.transfer(0x00);
  digitalWrite(CS,HIGH);

  digitalWrite(CS,LOW);
  SPI.transfer(0x13);
  SPI.transfer(0x04);
  digitalWrite(CS,HIGH);

    digitalWrite(CS,LOW);
  SPI.transfer(0x13);
  SPI.transfer(0x00);
  digitalWrite(CS,HIGH);
  callself();


}


void loop() {
 readtemp();
 recordAccelRegisters();
 plotdata();
}
void callself(){
  //call default who_am_i register
  digitalWrite(CS,LOW);
  SPI.transfer(0x8F);
  who=SPI.transfer(0x8F);
  digitalWrite(CS,HIGH);
  Serial.println(who);
}


void readtemp(){
  digitalWrite(CS,LOW);
  SPI.transfer(0xA0);
  tempL=SPI.transfer(0xA0);
  digitalWrite(CS,HIGH);
  
  digitalWrite(CS,LOW);
  SPI.transfer(0xA1);
  tempH=SPI.transfer(0xA1);
  digitalWrite(CS,HIGH);
  temperature=(~(tempL|tempH<<8)+1)/256+25;
}
void plotdata(){
  gForceX=(~(accelX0|accelX1<<8)+1)*0.061;
  gForceY=(~(accelY0|accelY1<<8)+1)*0.061;
  Serial.print("X:");
  Serial.print(gForceX);
  Serial.print("\t");
         
  Serial.print("Y:");
  Serial.print(gForceY);
 Serial.print("\t");
     Serial.print("Temperature:");    
  Serial.println(temperature);

  }

void recordAccelRegisters(){
   
  digitalWrite(CS,LOW);
  SPI.transfer(0xA8);
  accelX0=SPI.transfer(0xA8);
  digitalWrite(CS,HIGH);
  
  digitalWrite(CS,LOW);
  SPI.transfer(0xA9);
  accelX1= SPI.transfer(0xA9);
  digitalWrite(CS,HIGH);

  digitalWrite(CS,LOW);
  SPI.transfer(0xAA);
  accelY0=SPI.transfer(0xAA);
  digitalWrite(CS,HIGH);
  
  digitalWrite(CS,LOW);
  SPI.transfer(0xAB);
  accelY1=SPI.transfer(0xAB);
  digitalWrite(CS,HIGH);

 }


 
