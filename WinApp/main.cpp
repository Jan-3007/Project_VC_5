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


	return 0;
}