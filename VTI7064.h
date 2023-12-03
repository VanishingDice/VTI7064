#ifndef _VTI7064_H_
#define _VTI7064_H_

#include <Arduino.h>

#include <SPI.h>

// #define VTI7064_ADDR_MAX        (0x7FFFFF)//Not used
#define VTI7064_READ    (0x03)
#define VTI7064_WRITE   (0x02)
#define VTI7064_RESET_ENABLE    (0x66)
#define VTI7064_RESET   (0x99)

class VTI7064 {
public:
    VTI7064(uint8_t CE = 9);
    void begin(uint32_t clock = 20000000, uint8_t bitOrder = MSBFIRST, uint8_t dataMode = SPI_MODE0);
    void init();
    void startTransmission(uint8_t cmd, uint64_t addr);
    void end();

    void fullWrite(uint64_t addr, uint64_t len, uint8_t data);

    void write(uint64_t addr, uint8_t data);

    uint8_t read(uint64_t addr);

private:
    uint8_t _CE = 9;

};

#endif