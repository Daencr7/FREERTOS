#include <stdio.h>
#include <inttypes.h>
// #include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "freertos/timers.h"

#define mainONE_SHOT_TIMER_PERIOD       ( pdMS_TO_TICKS( 3333UL ) )
#define mainAUTO_RELOAD_TIMER_PERIOD    ( pdMS_TO_TICKS( 1000UL ) )

static void prvTimerCallback( TimerHandle_t xTimer );

static TimerHandle_t xAutoReloadTimer, xOneShotTimer;

int app_main( void )
{
    BaseType_t xTimer1Started, xTimer2Started;
    xOneShotTimer = xTimerCreate( "OneShot",                 /* Text name for the timer - not used by FreeRTOS. */
                                  mainONE_SHOT_TIMER_PERIOD, /* The timer's period in ticks. */
                                  pdFALSE,                   /* Set uxAutoRealod to pdFALSE to create a one-shot timer. */
                                  0,                         /* The timer ID is initialised to 0. */
                                  prvTimerCallback );        /* The callback function to be used by the timer being created. */

    xAutoReloadTimer = xTimerCreate( "AutoReload",                 /* Text name for the timer - not used by FreeRTOS. */
                                     mainAUTO_RELOAD_TIMER_PERIOD, /* The timer's period in ticks. */
                                     pdTRUE,                       /* Set uxAutoRealod to pdTRUE to create an auto-reload timer. */
                                     0,                            /* The timer ID is initialised to 0. */
                                     prvTimerCallback );           /* The callback function to be used by the timer being created. */

    /* Check the timers were created. */
    if( ( xOneShotTimer != NULL ) && ( xAutoReloadTimer != NULL ) )
    {
        xTimer1Started = xTimerStart( xOneShotTimer, 0 );
        xTimer2Started = xTimerStart( xAutoReloadTimer, 0 );
        if( ( xTimer1Started == pdPASS ) && ( xTimer2Started == pdPASS ) )
        {
            /* Start the scheduler. */
            // vTaskStartScheduler();
        }
    }
    for( ; ; )
    {
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    return 0;
}
/*-----------------------------------------------------------*/

static void prvTimerCallback( TimerHandle_t xTimer )
{
    TickType_t xTimeNow;
    uint32_t ulExecutionCount;
    ulExecutionCount = ( uint32_t ) pvTimerGetTimerID( xTimer );
    ulExecutionCount++;
    vTimerSetTimerID( xTimer, ( void * ) ulExecutionCount );
    xTimeNow = xTaskGetTickCount();
    if( xTimer == xOneShotTimer )
    {
        printf( "One-shot timer callback executing %ld\n", xTimeNow );
    }
    else
    {
        printf( "Auto-reload timer callback executing %ld\n", xTimeNow );

        if( ulExecutionCount == 5 )
        {
            xTimerStop( xTimer, 0 );
        }
    }
}
