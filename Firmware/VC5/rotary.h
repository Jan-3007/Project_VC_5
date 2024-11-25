#pragma once



class Rotary
{
protected:
    SemaphoreHandle_t sem_rotary_value_ {NULL};
    SemaphoreHandle_t sem_button_value_ {NULL};
    StaticSemaphore_t sem_buffer_rotary_value_;
    StaticSemaphore_t sem_buffer_button_value_;

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
    init_semaphores();

    void 
    update(uint16_t pin_state);

    void
    set_rot_value(int value);

    int
    get_rot_value();

    void
    set_btn_value(int value);

    int
    get_btn_value();

};






