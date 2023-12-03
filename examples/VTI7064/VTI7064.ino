/*
Uno pin:
10 CS
11 MOSI
12 MISO
13 CLK

Mega2560 pin:
50 MISO
51 MOSI
52 CLK
53 CS

This program use standard hardware SPI to fill VTI7064 from data[] and check is it right

2023.12 Jimmy Zhang
*/



#include "VTI7064.h"

#define DATA_SIZE 1000//data array size for check
#define TEST_LEN 0x800000 //number of test byte for VTI7064,max ??(A[22:0] in document)TODO

uint8_t dataReg;//register for the reading data, use for compare
uint8_t data[DATA_SIZE];//test data
uint8_t cycleCount = 0;
VTI7064 vti = VTI7064(53);//CS pin for VTI7064, should be 10 for Uno or 53 for Mega2560

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  vti.begin();
}

void loop() {
  randTest();
  // while(1) {}//if only needs to run one cycle
}

void randTest() {
  ++cycleCount;//Test cycle counter
  Serial.println("Test cycle "+String(cycleCount)+" Start.");
  
  Serial.println("Init data.");
  // initRandData();//Use random data to initialization data array
  initSeqData();//Use sequence data to initializaion data array
  Serial.println("Init data done.");

  Serial.println("Start writing.");
  for(uint16_t addr = 0; addr < TEST_LEN; ++addr) {
    vti.write(addr, data[addr%DATA_SIZE]);
    if(addr % 10000 == 0) {
      Serial.println(addr);
    }
  }
  Serial.println("Writing done.");

  Serial.println("Start reading");
  for(uint16_t addr = 0; addr < TEST_LEN; ++addr) {
    dataReg = vti.read(addr);
    if(dataReg == data[addr%DATA_SIZE]) {
      // Serial.println("Equal.");
    } else {
      Serial.println("ERROR: Address:"+String(addr)+"\t Value:"+dataReg);
      while(1){}
    }
  }
  Serial.println("Reading done.");

  Serial.println("Test cycle "+String(cycleCount)+" done.\n");

}

void initRandData() {
  for(uint16_t i = 0; i < DATA_SIZE; ++i) {
    data[i] = random(0,256);
  }
}

void initSeqData() {
  for(uint16_t i = 0; i < DATA_SIZE; ++i) {
    data[i] = i;
  }
}
