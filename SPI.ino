#include <SPI.h>
const int CS=10;
long accelX0, accelY0, accelZ0;
long accelX1, accelY1, accelZ1;
float gForceX,gForceY,gForceZ;

void setup() {
  Serial.begin(9600);
  pinMode(CS,OUTPUT); 
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE2);
  SPI.begin();
  setupData();
}

void setupData(){
  
 
  digitalWrite(CS,LOW);
  SPI.transfer(0x20);
  SPI.transfer(0x80);
  digitalWrite(CS,HIGH);
  

}


void loop() {

 recordAccelRegisters();
 plotdata();
}
void temp(){
//  digitalWrite(CS,LOW);
//  SPI.transfer(0x25);
//  byte tempL=SPI.transfer(0x25);
//  digitalWrite(CS,HIGH);
//  digitalWrite(CS,LOW);
//  SPI.transfer(0x26);
//  byte tempH=SPI.transfer(0x26);
//  digitalWrite(CS,HIGH);
//  long temp=((~(tempH<<8|tempL)+1)>>4)+25;
//  Serial.println(tempL,BIN);
//  Serial.println(tempH,BIN);
//  Serial.println(temp);
//  delay(1000);
//digitalWrite(CS,HIGH);  
}
void plotdata(){
  gForceX=(~(accelX0|accelX1<<8)+1)*0.076;
  gForceY=(~(accelY0|accelY1<<8)+1)*0.076;
  gForceZ=(~(accelZ0|accelZ1<<8)+1)*0.076;
  Serial.print("X:");
  Serial.print(gForceX);
  Serial.print("\t");
         
  Serial.print("Y:");
  Serial.print(gForceY);
  Serial.print("\t");
  
  Serial.print("Z:");
  Serial.println(gForceZ);
  }

void recordAccelRegisters(){
   
  digitalWrite(CS,LOW);
  SPI.transfer(0x28);
  accelX0=SPI.transfer(0xA8);
  digitalWrite(CS,HIGH);
  
  digitalWrite(CS,LOW);
  SPI.transfer(0x29);
  accelX1= SPI.transfer(0xA9);
  digitalWrite(CS,HIGH);
  
  digitalWrite(CS,LOW);
  SPI.transfer(0x2A);
  accelY0=SPI.transfer(0xAA);
  digitalWrite(CS,HIGH);
  
  digitalWrite(CS,LOW);
  SPI.transfer(0x2B);
  accelY1=SPI.transfer(0xAB);
  digitalWrite(CS,HIGH);
  
  digitalWrite(CS,LOW);
  SPI.transfer(0x2C);
  accelZ0=SPI.transfer(0xAC);
  digitalWrite(CS,HIGH);
  
  digitalWrite(CS,LOW);
  SPI.transfer(0x2D);
  accelZ1=SPI.transfer(0xAD);
  digitalWrite(CS,HIGH);

 }


 
