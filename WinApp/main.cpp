#include "VC5_global.h"





int
main()
{
	WinError err;

	GUID device_itf_guid = VOLCTRL_WINUSB_INTERFACE_GUID_INIT;

	// step 1
	std::vector<std::wstring> device_path_list;
	err = enumerate_devices(device_itf_guid, device_path_list);
	if (err != NO_ERROR)
	{
		return -1;
	}
	if (device_path_list.empty())
	{
		std::cout << std::format("No devices found.\n");
		return -1;
	}

	// step 2
	WinUSBDevice device;
	err = device.open(device_path_list[0].c_str());
	if (err != NO_ERROR)
	{
		return -1;
	}

	// step 3
	WinUSBInterface interface1;
	err = interface1.init(device.get_device_handle());
	if (err != NO_ERROR)
	{
		return -1;
	}

	// step 4
	WinUSBInterface interface2;
	err = interface2.get_associated_interface(interface1.get_itf_handle(), 1);
	if (err != NO_ERROR)
	{
		return -1;
	}

	// step 5
	err = interface2.set_pipe_transfer_timeout(EPNUM_VENDOR_2_INT_IN, 1000);
	if (err != NO_ERROR)
	{
		return -1;
	}

	std::cout << std::format("Reading events ... press any key to exit.\n");


	while (true)
	{
		Volctrl_EventMsg event_buffer;
		size_t len_transferred = 0;

		err = interface2.read_pipe_sync(EPNUM_VENDOR_2_INT_IN, &event_buffer, sizeof(event_buffer), len_transferred);
		if (err != NO_ERROR)
		{
			return -1;
		}

		if (len_transferred != 0)
		{
			if (len_transferred == sizeof(Volctrl_EventMsg))
			{
				std::cout << std::format("Event received: Code = {}, Rot num = {}, value = {}\n",
					event_buffer.event_code, event_buffer.rotary_num, event_buffer.value);
			}
			else
			{
				// unexpected length
				std::cout << std::format("unexpected length = {}\n", len_transferred);
			}
		}

		if (_kbhit())
		{
			_getch();
			break;
		}
	}


	return 0;
}