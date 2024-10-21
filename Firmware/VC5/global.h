#pragma once


//--------------------
// C++
//--------------------
#include <stdio.h>



//--------------------
// Pico SDK
//--------------------
#include "pico/stdlib.h"
#include "RP2040.h"
#include <hardware/i2c.h>



// check consistency with __NVIC_PRIO_BITS given in FreeRTOSConfig.h
#if (__NVIC_PRIO_BITS != 2)
    #error unexpected value for __NVIC_PRIO_BITS
#endif

//--------------------
// FreeRTOS
//--------------------
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>

#if LIB_PICO_MULTICORE
    #error dont want to support multicore
#endif



//--------------------
// Custom
//--------------------
#include "config.h"
#include "VC5_RP2040.h"
#include "MCP23017.h"




#ifdef __cplusplus
extern "C"
#endif
void fatal_error(int reason);