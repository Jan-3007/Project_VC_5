#include "global.h"



/*-----------------------------------------------------------*/

static void exampleTask( void * parameters ) __attribute__( ( noreturn ) );

/*-----------------------------------------------------------*/

static void exampleTask( void * parameters )
{
    /* Unused parameters. */
    ( void ) parameters;

    int count = 0;

    for( ; ; )
    {
        /* Example Task Code */
        printf("Hello World! %d \n", count++);
        vTaskDelay( 1000 ); /* delay 100 ticks */
    }
}
/*-----------------------------------------------------------*/

int main( void )
{
    stdio_init_all();

    ( void ) printf("VC 5: %s %s\n", __DATE__, __TIME__);







    static StaticTask_t exampleTaskTCB;
    static StackType_t exampleTaskStack[ configMINIMAL_STACK_SIZE ];

    ( void ) xTaskCreateStatic( exampleTask,
                                "example",
                                configMINIMAL_STACK_SIZE,
                                NULL,
                                configMAX_PRIORITIES - 1U,
                                &( exampleTaskStack[ 0 ] ),
                                &( exampleTaskTCB ) );

    /* Start the scheduler. */
    vTaskStartScheduler();

    for( ; ; )
    {
        /* Should not reach here. */
    }

    return 0;
}
/*-----------------------------------------------------------*/

#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )

    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName )
    {
        /* Check pcTaskName for the name of the offending task,
         * or pxCurrentTCB if pcTaskName has itself been corrupted. */
        ( void ) xTask;
        ( void ) pcTaskName;
    }

#endif /* #if ( configCHECK_FOR_STACK_OVERFLOW > 0 ) */
/*-----------------------------------------------------------*/
