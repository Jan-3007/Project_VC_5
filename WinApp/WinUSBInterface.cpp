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


WinError
WinUSBInterface::set_pipe_transfer_timeout(uint8_t pipe_id, uint32_t millisecs)
{
	return set_pipe_policy(pipe_id, PIPE_TRANSFER_TIMEOUT, &millisecs, sizeof(millisecs));
}


WinError
WinUSBInterface::set_pipe_policy(uint8_t pipe_id, uint32_t policy_type, void* val, uint32_t len)
{
	BOOL succ = ::WinUsb_SetPipePolicy(
		itf_handle_,			//		[in] WINUSB_INTERFACE_HANDLE InterfaceHandle,
		pipe_id,				//		[in] UCHAR                   PipeID,
		policy_type,			//		[in] ULONG                   PolicyType,
		len,					//		[in] ULONG                   ValueLength,
		val						//		[in] PVOID                   Value
	);
	if (!succ)
	{
		WinError err = ::GetLastError();
		std::cout << std::format("WinUsb_SetPipePolicy failed, err = {}", err);
		return err;
	}

	return NO_ERROR;
}



WinError
WinUSBInterface::read_pipe_sync(uint8_t pipe_id, void* buffer, size_t buf_len, size_t& len_transferred)
{
	ULONG b_count = 0;

	BOOL succ = ::WinUsb_ReadPipe(
		itf_handle_,					//	[in] WINUSB_INTERFACE_HANDLE InterfaceHandle,
		pipe_id,						//	[in]            UCHAR                   PipeID,
		static_cast<UCHAR*>(buffer),	//	[out]           PUCHAR                  Buffer,
		static_cast<ULONG>(buf_len),	//	[in]            ULONG                   BufferLength,
		&b_count,						//	[out, optional] PULONG                  LengthTransferred,
		nullptr							//	[in, optional]  LPOVERLAPPED            Overlapped
	);
	if (!succ)
	{
		WinError err = ::GetLastError();
		if (err == ERROR_SEM_TIMEOUT)
		{
			// Timeout, not an error
			b_count = 0;
		}
		else
		{
			std::cout << std::format("WinUsb_ReadPipe failed, err = {}", err);
			return err;
		}
	}

	len_transferred = b_count;

	return NO_ERROR;
}
