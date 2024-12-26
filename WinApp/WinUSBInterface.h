#pragma once

#include "VC5_global.h"



class WinUSBInterface
{
protected:
	WINUSB_INTERFACE_HANDLE itf_handle_{ INVALID_HANDLE_VALUE };

public:
	// ctor
	WinUSBInterface();
	// dtor
	~WinUSBInterface();

	// check if handle is valid
	bool
	is_valid() const;

	// get interface handle
	WINUSB_INTERFACE_HANDLE
	get_itf_handle() const
	{
		return itf_handle_;
	}

	// initialise WinUSB, get an interface handle
	WinError
	init(HANDLE device_handle);

	// get associated interface
	// alternative to init!!!
	WinError
	get_associated_interface(WINUSB_INTERFACE_HANDLE itf_handle, uint8_t itf_index);

	// free WinUSB interface handle, if valid
	WinError
	free();
};

