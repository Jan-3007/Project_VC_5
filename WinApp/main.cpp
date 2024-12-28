#include "VC5_global.h"





int
main()
{
	WinError err;

	GUID device_itf_guid = VC5_WINUSB_INTERFACE_GUID_INIT;

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
	err = interface2.get_associated_interface(interface1.get_itf_handle(), 0);
	if (err != NO_ERROR)
	{
		return -1;
	}

	// step 5: configure pipe policies
	err = interface1.set_pipe_autoclear_stall(EPNUM_VENDOR_1_BULK_IN, TRUE);
	if (err != NO_ERROR)
	{
		return -1;
	}

	err = interface2.set_pipe_autoclear_stall(EPNUM_VENDOR_2_INT_IN, TRUE);
	if (err != NO_ERROR)
	{
		return -1;
	}

	err = interface2.set_pipe_transfer_timeout(EPNUM_VENDOR_2_INT_IN, 100);
	if (err != NO_ERROR)
	{
		return -1;
	}

	// remove any old data from device pipe fifo
	err = interface2.reset_pipe(EPNUM_VENDOR_2_INT_IN);
	if (err != NO_ERROR)
	{
		return -1;
	}

	std::cout << std::format("Reading events ... press any key to exit.\n");


	while (true)
	{
		VC5_EventMsg event_buffer;
		size_t len_transferred = 0;

		err = interface2.read_pipe_sync(EPNUM_VENDOR_2_INT_IN, &event_buffer, sizeof(event_buffer), len_transferred);
		if (err != NO_ERROR)
		{
			return -1;
		}

		if (len_transferred != 0)
		{
			if (len_transferred == sizeof(VC5_EventMsg))
			{
				std::cout << std::format("Event received: Code = {}, Rot num = {}, value = {}\n",
					event_buffer.event_code, event_buffer.rotary_index, event_buffer.value);
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

		Sleep(100);
	}


	return 0;
}