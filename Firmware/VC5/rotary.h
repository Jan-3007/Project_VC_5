#pragma once



class Rotary
{
protected:
    uint rotary_index_ {0};
    int rotary_value_ {0};
    int button_value_ {0};
    uint16_t clk_mask_;
    uint16_t data_mask_;
    uint16_t button_mask_;

	bool prev_clk_state_ {true};
    int prev_btn_state_ {true};
	bool reverse_cnt_dir_ {false};      

public:
    // ctor
    Rotary(uint index, uint16_t clk, uint16_t data, uint16_t button, bool reverse)
        : rotary_index_ {index}, clk_mask_ {clk}, data_mask_ {data}, button_mask_ {button}, reverse_cnt_dir_ {reverse}
        {}

    void 
    update(uint16_t pin_state);
};






