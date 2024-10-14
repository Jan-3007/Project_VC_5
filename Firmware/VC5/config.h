//-----------------------------------------------------------
// Raspberry Pi Pico - I²C
//-----------------------------------------------------------

#define I2C_PORT i2c1
constexpr int c_i2c_sda_pin = 6;
constexpr int c_i2c_scl_pin = 7;
// freq = 400 kHz
constexpr int c_i2c_freq_hz = 400 * 1000;