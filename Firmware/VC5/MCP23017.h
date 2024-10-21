#pragma once


class MCP23017
{
protected:
    static constexpr uint8_t slave_address = 0x40 >> 1;

    // bitmask for rotary CLK pins
    static constexpr uint16_t clk_pin_mask = 0
                | (1u << 1)
                | (1u << 4)
                | (1u << 15)
                | (1u << 10)
                | (1u << 12);

    // bitmask for rotary CLK pins
    static constexpr uint16_t button_pin_mask = 0
                | (1u << 2)
                | (1u << 3)
                | (1u << 6)
                | (1u << 14)
                | (1u << 13);


public:
    //ctor
    MCP23017();
    
    // Initialise MCP23017
    void init();

    // bit 0..7: Port A
    // bit 8..15: Port B
    uint16_t get_pin_state();

protected:
    // function to write a 16 bit value to a register of MCP23017 via I²C
    bool write_register16(uint8_t reg_addr, uint16_t value);

    // function to read a 16 bit value to a register of MCP23017 via I²C
    bool read_register16(uint8_t reg_addr, uint16_t& value);

    void enable_interrupt();

    void configure_int_on_falling_edge();


};

