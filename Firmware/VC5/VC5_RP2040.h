#pragma once


//--------------------
// I²C
//--------------------
void 
init_i2c();

int 
write_i2c(uint8_t addr, const uint8_t* src, size_t len, bool nostop);

int 
read_i2c(uint8_t addr, uint8_t* dst, size_t len, bool nostop);
