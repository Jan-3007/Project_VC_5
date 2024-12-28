#include "VC5_global.h"


// ctor
VC5Application::VC5Application()
{

}


WinError 
VC5Application::init_WinUSB()
{
	GUID device_itf_guid = VC5_WINUSB_INTERFACE_GUID_INIT;

	// step 1: enumerate WinUSB devices, filter with device guid
	std::vector<std::wstring> device_path_list;
	WinError err = enumerate_devices(device_itf_guid, device_path_list);
	if (err != NO_ERROR)
	{
		return err;
	}
	if (device_path_list.empty())
	{
		std::cout << std::format("{}: No devices found.\n", __func__);
		return ERROR_ERRORS_ENCOUNTERED;
	}

	// step 2: open device
	err = device_.open(device_path_list[0].c_str());
	if (err != NO_ERROR)
	{
		return err;
	}

	// step 3: get default interface, interface 0
	err = interface1_.init(device_.get_device_handle());
	if (err != NO_ERROR)
	{
		return err;
	}

	// step 4: get second interface, first associated itf is at pos 0
	err = interface2_.get_associated_interface(interface1_.get_itf_handle(), 0);
	if (err != NO_ERROR)
	{
		return err;
	}

	// step 5: init pipe policies and remove any old data from device IN pipe fifo
	err = init_pipe_policies();
	if (err != NO_ERROR)
	{
		return err;
	}

	return NO_ERROR;
}


WinError 
VC5Application::init_pipe_policies()
{
/*
	WinError err = interface1_.set_pipe_autoclear_stall(EPNUM_VENDOR_1_BULK_IN, TRUE);
	if (err != NO_ERROR)
	{
		return err;
	}

	err = interface2_.set_pipe_autoclear_stall(EPNUM_VENDOR_2_INT_IN, TRUE);
	if (err != NO_ERROR)
	{
		return err;
	}
*/
	WinError err = interface2_.set_pipe_transfer_timeout(EPNUM_VENDOR_2_INT_IN, 100);
	if (err != NO_ERROR)
	{
		return err;
	}

	// TODO: remove any old data from device IN pipe fifo

	return NO_ERROR;
}


WinError 
VC5Application::check_event()
{
	// read pipe
	size_t len_transferred = 0;
	WinError err = interface2_.read_pipe_sync(EPNUM_VENDOR_2_INT_IN, &event_buffer_, sizeof(event_buffer_), len_transferred);
	if (err != NO_ERROR)
	{
		return err;
	}

	if (len_transferred == 0)
	{
		// read timed out
		return NO_ERROR;
	}

	if (len_transferred != sizeof(VC5_EventMsg))
	{
		// unexpected length
		std::cout << std::format("{}: Vendor 2: unexpected length = {}\n", __func__, len_transferred);
		return ERROR_ERRORS_ENCOUNTERED;
	}

	// check rotary index
	if (event_buffer_.rotary_index >= c_num_rotaries)
	{
		// not a valid index
		std::cout << std::format("{}: Vendor 2: unexpected index = {}\n", __func__, event_buffer_.rotary_index);
		return ERROR_ERRORS_ENCOUNTERED;
	}

	// process data from pipe
//	std::cout << std::format("Event received: Code = {}, Rot index = {}, value = {}\n",
//		event_buffer_.event_code, event_buffer_.rotary_index, event_buffer_.value);

	err = NO_ERROR;
	switch (event_buffer_.event_code)
	{
		case VC5_EventMsg::evt_rotary_turned:
			// on rotary turned
			if(event_buffer_.value != 0)
			{
				err = update_volume();
			}
			break;

	
		case VC5_EventMsg::evt_rotary_button:
			// on button released
			if (event_buffer_.value == -1)
			{
				err = toggle_mute();
			}
			else
			{
				//std::cout << std::format("btn pressed\n");
			}
			break;


		default:
			// unknown event code
			std::cout << std::format("{}: Vendor 2: unknown event code = {}\n", __func__, event_buffer_.event_code);
			return ERROR_ERRORS_ENCOUNTERED;
	}

	return err;
}


WinError 
VC5Application::toggle_mute()
{
	VC5Unit& unit = VC5_units_[event_buffer_.rotary_index];

	if (unit.is_mute())
	{
		// clear mute
		unit.set_mute(false);
	}
	else
	{
		// set mute
		unit.set_mute(true);
	}

	std::cout << std::format("{}: rot = {}, volume = {}, mute = {}\n", __func__, event_buffer_.rotary_index, unit.get_volume(), unit.is_mute());

//	WinError err = update_led();

	return NO_ERROR;
}


WinError 
VC5Application::update_volume()
{
	VC5Unit& unit = VC5_units_[event_buffer_.rotary_index];

	// increase or decrease volume
	unit.change_volume(event_buffer_.value);

	// update mute after volume change
	unit.update_mute();

	std::cout << std::format("{}: rot = {}, new volume = {}, mute = {}\n", __func__, event_buffer_.rotary_index, unit.get_volume(), unit.is_mute());

//	WinError err = update_led();
//	WinError err = update_display();

	return NO_ERROR;
}
