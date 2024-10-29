#include "VC5_global.h"


void 
Rotary::update(uint16_t pin_state)
{
    // true == High, false == Low
    bool current_clk_state = ((pin_state & clk_mask_) != 0);

    // detect falling edge
    if(prev_clk_state_ && !current_clk_state)
    {
        // true == High, false == Low
        bool data_state = ((pin_state & data_mask_) != 0);
        if(data_state)
        {
            if(reverse_cnt_dir_)
            {
                rotary_value_++;
            }
            else
            {
                rotary_value_--;
            }
        }
        else
        {
            if(reverse_cnt_dir_)
            {
                rotary_value_--;
            }
            else
            {
                rotary_value_++;
            }
        }
#if 1
        printf("R %d: %d\n", rotary_index_, rotary_value_);
#endif
    }
    prev_clk_state_ = current_clk_state;



    // true == High == released, false == Low == pressed
    bool current_btn_state = ((pin_state & button_mask_) != 0);

    // detect press
    if(prev_btn_state_ && !current_btn_state)
    {
        // button pressed
        button_value_ = 1;
    }

    // detect release
    if(!prev_btn_state_ && current_btn_state)
    {
        // button released
        button_value_ = -1;
    }
    
    prev_btn_state_ = current_btn_state;

#if 1
    if(button_value_ != 0)
    {
        printf("Rotary button %d: %d\n", rotary_index_, button_value_);
        button_value_ = 0;
    }
#endif
}

