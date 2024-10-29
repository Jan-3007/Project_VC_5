#include "VC5_global.h"



//--------------------
// I²C
//--------------------

void 
init_i2c()
{
    // I2C Initialisation
    i2c_init(I2C_PORT, c_i2c_freq_hz);
    
    gpio_set_function(c_i2c_sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(c_i2c_scl_pin, GPIO_FUNC_I2C);
    //gpio_pull_up(c_i2c_sda_pin);
    //gpio_pull_up(c_i2c_scl_pin);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c
}


int 
write_i2c(uint8_t addr, const uint8_t* src, size_t len, bool nostop)
{
    return i2c_write_blocking(I2C_PORT, addr, src, len, nostop);
}


int 
read_i2c(uint8_t addr, uint8_t* dst, size_t len, bool nostop)
{
    return i2c_read_blocking(I2C_PORT, addr, dst, len, nostop);
}
