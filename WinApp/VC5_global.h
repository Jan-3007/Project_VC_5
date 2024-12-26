#pragma once


// C++
#include <format>
#include <iostream>


// Windows
// exclude GUI stuff to speed up compilation
#define WIN32_LEAN_AND_MEAN	1
#include <Windows.h>
#include <SetupAPI.h>
#pragma comment(lib, "setupapi.lib")



// WinUSB
#include <winusb.h>
#pragma comment(lib, "winusb.lib")



// VC5
#include "WinUtils.h"
#include "usb_protocol.h"
#include "WinUSBInterface.h"
