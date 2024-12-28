#pragma once



class VC5Unit
{
protected:
	uint8_t led_index_;
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
			mute_ = true;
		}
		else
		{
			mute_ = false;
		}
	}
};

