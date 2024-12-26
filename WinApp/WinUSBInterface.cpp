#include "VC5_global.h"



// ctor
WinUSBInterface::WinUSBInterface()
{
	// empty
}


// dtor
WinUSBInterface::~WinUSBInterface()
{
	free();
}


bool
WinUSBInterface::is_valid() const
{
	return (itf_handle_ != INVALID_HANDLE_VALUE);
}


WinError
WinUSBInterface::init(HANDLE device_handle)
{
	if (is_valid())
	{
		std::cout << std::format("{}: interface handle exists", __func__);
		return ERROR_INTERNAL_ERROR;
	}

	WinError err = NO_ERROR;
	BOOL succ = ::WinUsb_Initialize(device_handle, &itf_handle_);
	if (!succ)
	{
		err = ::GetLastError();
		std::cout << std::format("WinUsb_Initialize failed, err = {}", err);
	}
	return err;
}


WinError
WinUSBInterface::get_associated_interface(WINUSB_INTERFACE_HANDLE itf_handle, uint8_t itf_index)
{
	if (is_valid())
	{
		std::cout << std::format("{}: interface handle exists", __func__);
		return ERROR_INTERNAL_ERROR;
	}

	WinError err = NO_ERROR;
	BOOL succ = ::WinUsb_GetAssociatedInterface(itf_handle, itf_index, &itf_handle_);
	if (!succ)
	{
		err = ::GetLastError();
		std::cout << std::format("WinUsb_GetAssociatedInterface failed, err = {}", err);
	}
	return err;

}


WinError
WinUSBInterface::free()
{
	WinError err = NO_ERROR;
	if (is_valid())
	{
		BOOL succ = ::WinUsb_Free(itf_handle_);
		if (!succ)
		{
			err = ::GetLastError();
			std::cout << std::format("WinUsb_Free failed, err = {}", err);
		}
		itf_handle_ = INVALID_HANDLE_VALUE;
	}
	return err;
}

