//#include <Arduino.h>
//#include <SPI.h>
#include "RM3100.h"
#include "atmel_stuff.h"
#include "serial_print.h"
#include <math.h>

float gain;

void CS_HIGH(int CS)
{
    digitalWrite(CS, HIGH);
    delay(100);
}

void CS_LOW(int CS)
{
    digitalWrite(CS, LOW);
    delay(100);
}

void setup() {
  pinMode(PIN_DRDY, INPUT);  
  pinMode(PIN_CS, OUTPUT);
  digitalWrite(PIN_CS, HIGH);
  
  //SPI.begin(); // Initiate the SPI library
  //SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));  
  //Serial.begin(9600); //set baud rate to 9600
  delay(100);

  revid = readReg(RM3100_REVID_REG);
  
  s_print("REVID ID = 0x"); //REVID ID should be 0x22
  s_print(revid, HEX);

  changeCycleCount(initialCC); //change the cycle count; default = 200 (lower cycle count = higher data rates but lower resolution)

  cycleCount = readReg(RM3100_CCX1_REG);
  cycleCount = (cycleCount << 8) | readReg(RM3100_CCX0_REG);

  s_print("Cycle Counts = "); //display cycle count
   s_print(cycleCount);

  gain = (0.3671 * (float)cycleCount) + 1.5; //linear equation to calculate the gain from cycle count

   s_print("Gain = "); //display gain; default gain should be around 75 for the default cycle count of 200
   s_print(gain);

  if (singleMode){
    //set up single measurement mode
    writeReg(RM3100_CMM_REG, 0);
    writeReg(RM3100_POLL_REG, 0x70);
  }
  else{
    // Enable transmission to take continuous measurement with Alarm functions off
    writeReg(RM3100_CMM_REG, 0x79);
  }
}

void loop() {
  long x = 0;
  long y = 0;
  long z = 0;
  uint8_t x2,x1,x0,y2,y1,y0,z2,z1,z0;

  //wait until data is ready using 1 of two methods (chosen in options at top of code)
  if(useDRDYPin){ 
    while(digitalRead(PIN_DRDY) == LOW); //check RDRY pin
  }
  else{
    while((readReg(RM3100_STATUS_REG) & 0x80) != 0x80); //read internal status register
  }
  
  //read measurements
  digitalWrite(PIN_CS, LOW);
  delay(100);
  spiSendReceive(0xA4);
  x2 = spiSendReceive(0xA5);
  x1 = spiSendReceive(0xA6);
  x0 = spiSendReceive(0xA7);
  
  y2 = spiSendReceive(0xA8);
  y1 = spiSendReceive(0xA9);
  y0 = spiSendReceive(0xAA);
  
  z2 = spiSendReceive(0xAB);
  z1 = spiSendReceive(0xAC);
  z0 = spiSendReceive(0);
  
  digitalWrite(PIN_CS, HIGH);

  //special bit manipulation since there is not a 24 bit signed int data type
  if (x2 & 0x80){
      x = 0xFF;
  }
  if (y2 & 0x80){
      y = 0xFF;
  }
  if (z2 & 0x80){
      z = 0xFF;
  }

  //format results into single 32 bit signed value
  x = (x * 256 * 256 * 256) | (int32_t)(x2) * 256 * 256 | (uint16_t)(x1) * 256 | x0;
  y = (y * 256 * 256 * 256) | (int32_t)(y2) * 256 * 256 | (uint16_t)(y1) * 256 | y0;
  z = (z * 256 * 256 * 256) | (int32_t)(z2) * 256 * 256 | (uint16_t)(z1) * 256 | z0;

  //calculate magnitude of results
  double uT = sqrt(pow(((float)(x)/gain),2) + pow(((float)(y)/gain),2)+ pow(((float)(z)/gain),2));

  //display results
  s_print("Data in counts:");
  s_print("   SeriaX:");
  s_print(x);
  s_print("   Y:");
  s_print(y);
  s_print("   Z:");
  s_printl(z);

  s_print("Data in microTesla(uT):");
  s_print("   X:");
  s_print((float)(x)/gain);
  s_print("   Y:");
  s_print((float)(y)/gain);
  s_print("   Z:");
  s_print((float)(z)/gain);

  //Magnitude should be around 45 uT (+/- 15 uT)
  s_print("Magnitude(uT):");
  s_print(uT);
  s_print();    
}

//addr is the 7 bit value of the register's address (without the R/W bit)
uint8_t readReg(uint8_t addr){
  /*uint8_t data = 0;
  digitalWrite(PIN_CS, LOW);
  delay(100);
  SPI.transfer(addr | 0x80); //OR with 0x80 to make first bit(read/write bit) high for read
  data = SPI.transfer(0);
  digitalWrite(PIN_CS, HIGH);
  return data;*/

  uint8_t data = 0;
  CS_LOW(PIN_CS);
  spiSendReceive(addr | 0x80);
  data = spiSendReceive(0x00);
  CS_HIGH(CS);
  return data;
}


//addr is the 7 bit (No r/w bit) value of the internal register's address, data is 8 bit data being written
void writeReg(uint8_t addr, uint8_t data){
  /*digitalWrite(PIN_CS, LOW); 
  delay(100);
  SPI.transfer(addr & 0x7F); //AND with 0x7F to make first bit(read/write bit) low for write
  SPI.transfer(data);
  digitalWrite(PIN_CS, HIGH);*/

  CS_LOW(CS);
  spiSendReceive(addr & 0x7F);
  spiSendReceive(data);
  CS_HIGH(CS);
}


//newCC is the new cycle count value (16 bits) to change the data acquisition
void changeCycleCount(uint16_t newCC){
  uint8_t CCMSB = (newCC & 0xFF00) >> 8; //get the most significant byte
  uint8_t CCLSB = newCC & 0xFF; //get the least significant byte
    
  digitalWrite(PIN_CS, LOW); 
  delay(100);
  spiSendReceive(RM3100_CCX1_REG & 0x7F); //AND with 0x7F to make first bit(read/write bit) low for write
  spiSendReceive(CCMSB);  //write new cycle count to ccx1
  spiSendReceive(CCLSB);  //write new cycle count to ccx0
  spiSendReceive(CCMSB);  //write new cycle count to ccy1
  spiSendReceive(CCLSB);  //write new cycle count to ccy0
  spiSendReceive(CCMSB);  //write new cycle count to ccz1
  spiSendReceive(CCLSB);  //write new cycle count to ccz0
  digitalWrite(PIN_CS, HIGH);
}