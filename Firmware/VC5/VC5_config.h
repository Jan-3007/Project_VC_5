#pragma once



//-----------------------------------------------------------
// VC 5 - General
//-----------------------------------------------------------

constexpr uint c_num_units = 5;         // 1 unit = 1 rotary + 1 display + 1 LED



//-----------------------------------------------------------
// Raspberry Pi Pico - Pins
//-----------------------------------------------------------

constexpr uint c_i2c_sda_pin = 6;
constexpr uint c_i2c_scl_pin = 7;

constexpr uint c_mcp_int_pin = 3;



//-----------------------------------------------------------
// Raspberry Pi Pico - I²C
//-----------------------------------------------------------

#define I2C_PORT i2c1
// freq = 400 kHz
constexpr int c_i2c_freq_hz = 400 * 1000;



//-----------------------------------------------------------
// FreeRTOS
//-----------------------------------------------------------

// RP2040 SRAM size: 264kB on-chip
// Pico Flash size: 2MB on-board

namespace task_rotaries
{
    const char name[] = "ROT";
    constexpr uint c_prio = configMAX_PRIORITIES - 1u;
    constexpr uint c_stack_size_bytes = 1024;
    constexpr uint c_stack_size_words = c_stack_size_bytes / 4;
}

namespace task_usb
{
    const char name[] = "USB";
    constexpr uint c_prio = configMAX_PRIORITIES - 2u;
    constexpr uint c_stack_size_bytes = 4096;
    constexpr uint c_stack_size_words = c_stack_size_bytes / 4;

    constexpr uint c_usb_task_interval_ms = 10;
}



//-----------------------------------------------------------
// Rotaries + MCP23017
//-----------------------------------------------------------

struct rotary_pins_mask
{
    uint16_t clk_mask {0};
    uint16_t data_mask {0};
    uint16_t button_mask {0};
};

// rotary masks
//                               clk,       data,      button
constexpr rotary_pins_mask rot_0 {(1u << 1), (1u << 0), (1u << 2)};
constexpr rotary_pins_mask rot_1 {(1u << 4), (1u << 5), (1u << 3)};
constexpr rotary_pins_mask rot_2 {(1u << 15), (1u << 7), (1u << 6)};
constexpr rotary_pins_mask rot_3 {(1u << 10), (1u << 9), (1u << 14)};
constexpr rotary_pins_mask rot_4 {(1u << 12), (1u << 11), (1u << 13)};


constexpr uint c_num_rotaries = c_num_units;


constexpr uint c_debounce_threshold = 2;
constexpr uint c_debounce_delay_ms = 1;




