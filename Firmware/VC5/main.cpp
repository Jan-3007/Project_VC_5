#include "VC5_global.h"



#ifdef __cplusplus
extern "C"
#endif
void fatal_error(int reason)
{
    printf("FATAL ERROR: reason %d", reason);

    while(true);
}



int main()
{
    stdio_init_all();
    printf("VC 5: %s %s\n", __DATE__, __TIME__);

    init_i2c();

    // init and task setup for the rotaries
    init_rotaries();

    // init and task setup for USB
    init_usb();



    /* Start the scheduler. */
    vTaskStartScheduler();

    for( ; ; )
    {
        /* Should not reach here. */
    }

    return 0;
}