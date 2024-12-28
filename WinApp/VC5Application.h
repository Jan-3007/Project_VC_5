#pragma once




class VC5Application
{
protected:
	// WinUSB
	WinUSBDevice device_;
	WinUSBInterface interface1_;
	WinUSBInterface interface2_;

	// for event data transmission
	VC5_EventMsg event_buffer_{0};

	// VC5 units
	VC5Unit VC5_units_[c_num_units]
		{
			//led index, display index
			{0, 0}, // = Rotary 0
			{1, 1}, // = Rotary 1
			{2, 2}, // = Rotary 2
			{3, 3}, // = Rotary 3
			{4, 4}  // = Rotary 4 
		};

public:
	// ctor
	VC5Application();

	WinError
	init_WinUSB();

	WinError
	check_event();

protected:
	WinError
	init_pipe_policies();

	WinError
	toggle_mute();

	WinError
	update_volume();
};

