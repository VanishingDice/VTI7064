#include "Arduino.h"
#include "VTI7064.h"

VTI7064::VTI7064(uint8_t CE) {
    _CE = CE;
}

void VTI7064::begin(uint32_t clock , uint8_t bitOrder , uint8_t dataMode) {
    pinMode(_CE, OUTPUT);
    digitalWrite(_CE, HIGH);
    SPI.beginTransaction(SPISettings(clock, bitOrder, dataMode));
    SPI.begin();
    delayMicroseconds(150);
    init();
}

void VTI7064::init() {
    digitalWrite(_CE, LOW);
    SPI.transfer(VTI7064_RESET_ENABLE);
    digitalWrite(_CE, HIGH);
    digitalWrite(_CE, LOW);
    SPI.transfer(VTI7064_RESET);
    digitalWrite(_CE, HIGH);
}

void VTI7064::startTransmission(uint8_t cmd, uint64_t addr) {
    SPI.transfer(cmd);
    SPI.transfer(addr>>16 & B11111111);
    SPI.transfer(addr>>8 & B11111111);
    SPI.transfer(addr & B11111111);
}

void VTI7064::end() {
    SPI.endTransaction();
}

void VTI7064::fullWrite(uint64_t addr, uint64_t len, uint8_t data) {
    digitalWrite(_CE, LOW);
    startTransmission(VTI7064_WRITE, addr);
    while(len > 0) {
        SPI.transfer(data);
        --len;
    }
    digitalWrite(_CE, HIGH);
}

void VTI7064::write(uint64_t addr, uint8_t data) {
    digitalWrite(_CE, LOW);
    startTransmission(VTI7064_WRITE, addr);
    SPI.transfer(data);
    digitalWrite(_CE, HIGH);
}

uint8_t VTI7064::read(uint64_t addr) {
    digitalWrite(_CE, LOW);
    startTransmission(VTI7064_READ, addr);
    uint8_t data = SPI.transfer(B11111111);//Manual said that this data can be anything, but only B11111111 works...
    digitalWrite(_CE, HIGH);
    return data;
}

