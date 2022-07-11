//pin connection
//Arudino SPI interface
//sensor board INT1 and INT2 connect to GND
//SPC -> Clk
//SDI -> MOSI(COPI)
//SDO -> MISO(CIPO)
//CS -> SS(CS)
//documentation link https://www.st.com/resource/en/datasheet/iis3dhhc.pdf
//not FIFO output


#include <SPI.h>
const int CS = 10;
byte accelXL, accelYL, accelZL, tempL, tempH;
byte accelXH, accelYH, accelZH, data;
double gForceX, gForceY, gForceZ, temperature;
byte who;

void setup() {
  Serial.begin(9600);
  pinMode(CS, OUTPUT);
  //SPI.setBitOrder(MSBFIRST);
  //SPI.setDataMode(SPI_MODE3);

  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3));
  SPI.endTransaction();
  SPI.begin();
  setupData();
}

void setupData() {

  //config device only SPI
  //read = register add | 0x80
  //write = register
  //20h
  digitalWrite(CS, LOW);
  SPI.transfer(0x20);//
  SPI.transfer(0x80);
  digitalWrite(CS, HIGH);

  
  //21h
  digitalWrite(CS, LOW);
  SPI.transfer(0x21);
  SPI.transfer(0x00);
  digitalWrite(CS, HIGH);

  //22h
  digitalWrite(CS, LOW);
  SPI.transfer(0x22);
  SPI.transfer(0x00);
  digitalWrite(CS, HIGH);

  //23h
  digitalWrite(CS, LOW);
  SPI.transfer(0x23);
  SPI.transfer(0x00);
  digitalWrite(CS, HIGH);

  //24h
  digitalWrite(CS, LOW);
  SPI.transfer(0x24);
  SPI.transfer(0x00);
  digitalWrite(CS, HIGH);

  //2Eh
  digitalWrite(CS, LOW);
  SPI.transfer(0x2E);
  SPI.transfer(0x00);
  digitalWrite(CS, HIGH);


}


void loop() {
  //callself();
  readtemp();
  recordAccelRegisters();
  plotdata();
}
void callself() {
  //call default who_am_i register
  digitalWrite(CS, LOW);
  SPI.transfer(0x8F);// 0x0F|0x80
  who = SPI.transfer(0x8F);
  digitalWrite(CS, HIGH);
  Serial.println(who);
}


void readtemp() {
  //Temp_L(25h) Temp_H(26h)
  digitalWrite(CS, LOW);
  //SPI.transfer(0xA5);
  tempL = SPI.transfer(0xA5);
  digitalWrite(CS, HIGH);

  digitalWrite(CS, LOW);
  //SPI.transfer(0xA6);
  tempH = SPI.transfer(0xA6);
  digitalWrite(CS, HIGH);
  //two's complement with 0LSB=25degree and 16 digit/degree
  temperature = (~(tempL | tempH << 8) + 1) * 16 + 25;
}
void plotdata() {
  //change accleration into gForce with 2's complement, sensitivity 0.076mg/digit
  gForceX = (~(accelXL | accelXH << 8) + 1) * 0.076;
  gForceY = (~(accelYL | accelYH << 8) + 1) * 0.076;
  gForceY = (~(accelZL | accelZH << 8) + 1) * 0.076;

  //print data
  Serial.print("X:");
  Serial.print(gForceX);
  Serial.print("\t");

  Serial.print("Y:");
  Serial.print(gForceY);
  Serial.print("\t");

  Serial.print("Z:");
  Serial.print(gForceZ);
  Serial.print("\t");
  Serial.print("Temperature:");
  Serial.println(temperature);

}

void recordAccelRegisters() {
  //record acceleration X
  digitalWrite(CS, LOW);
  SPI.transfer(0xA8);
  accelXL = SPI.transfer(0xA8);
  digitalWrite(CS, HIGH);

  digitalWrite(CS, LOW);
  SPI.transfer(0xA9);
  accelXH = SPI.transfer(0xA9);
  digitalWrite(CS, HIGH);

  //record acceleration Y
  digitalWrite(CS, LOW);
  SPI.transfer(0xAA);
  accelYL = SPI.transfer(0xAA);
  digitalWrite(CS, HIGH);

  digitalWrite(CS, LOW);
  SPI.transfer(0xAB);
  accelYH = SPI.transfer(0xAB);
  digitalWrite(CS, HIGH);

  //record acceleration Z
  digitalWrite(CS, LOW);
  SPI.transfer(0xAC);
  accelZL = SPI.transfer(0xAC);
  digitalWrite(CS, HIGH);

  digitalWrite(CS, LOW);
  SPI.transfer(0xAD);
  accelZH = SPI.transfer(0xAD);
  digitalWrite(CS, HIGH);
}
