#pragma once



class VC5Unit
{
protected:
	// LED
	uint8_t led_index_;
	ColorTemplate led_color_ {color::black};

	uint8_t display_index_;

	// current state
	bool mute_ {false};
	uint volume_ {0};

public:
	// ctor
	VC5Unit(uint8_t led_index, uint8_t display_index)
		: led_index_{led_index}, display_index_{display_index}
	{}

	bool
	is_mute()
		{
			return mute_;
		}

	void
	set_mute(bool val)
		{
			mute_ = val;
			if(val)
			{
				// mute active
				led_color_ = mute_on;
			}
			else
			{
				// mute inactive
				led_color_ = mute_off;
			}
		}

	uint
	get_volume()
		{
			return volume_;
		}
	
	void
	change_volume(int8_t val)
		{
			int new_vol = volume_ + val;
			if (new_vol >= 0 && new_vol <= 100)
			{
				volume_ = new_vol;
			}
			else if (new_vol > 100)
			{
				volume_ = 100;
			}
			else
			{
				volume_ = 0;
			}
		}

	void
	update_mute()
		{
			if (volume_ == 0)
			{
				set_mute(true);
			}
			else
			{
				set_mute(false);
			}
		}

	uint8_t
	get_led_index()
		{
			return led_index_;
		}

	ColorTemplate
	get_led_color()
		{
			return led_color_;
		}

};

