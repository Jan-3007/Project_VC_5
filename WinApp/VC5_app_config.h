#pragma once


//-----------------------------------------------------------
// VC 5 - General
//-----------------------------------------------------------

constexpr uint32_t c_num_units = 5;         // 1 unit = 1 rotary + 1 display + 1 LED
constexpr uint32_t c_num_rotaries = c_num_units;

//-----------------------------------------------------------
// VC 5 - Usage
//-----------------------------------------------------------

// true: if volume is changed, mute will be set according to the new value
// false: mute remains when volume is changed
constexpr bool c_clear_mute_on_change = false;
