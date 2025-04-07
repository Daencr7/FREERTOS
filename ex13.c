/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
// #include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "freertos/timers.h"
// #include "FreeRTOSConfig.h"  

/* Demo includes. */
#include "supporting_functions.h"

/* The periods assigned to the one-shot and auto-reload timers respectively. */
#define mainONE_SHOT_TIMER_PERIOD       ( pdMS_TO_TICKS( 3333UL ) )
#define mainAUTO_RELOAD_TIMER_PERIOD    ( pdMS_TO_TICKS( 500UL ) )

static void prvOneShotTimerCallback( TimerHandle_t xTimer );
static void prvAutoReloadTimerCallback( TimerHandle_t xTimer );

/*-----------------------------------------------------------*/
int app_main(void)
{
    TimerHandle_t xAutoReloadTimer, xOneShotTimer;
    BaseType_t xTimer1Started, xTimer2Started;

    printf("app_main started\n");

    xOneShotTimer = xTimerCreate("OneShot", mainONE_SHOT_TIMER_PERIOD, pdFALSE, 0, prvOneShotTimerCallback);
    xAutoReloadTimer = xTimerCreate("AutoReload", mainAUTO_RELOAD_TIMER_PERIOD, pdTRUE, 0, prvAutoReloadTimerCallback);

    if ((xOneShotTimer != NULL) && (xAutoReloadTimer != NULL)) {
        xTimer1Started = xTimerStart(xOneShotTimer, 0);
        xTimer2Started = xTimerStart(xAutoReloadTimer, 0);

        if ((xTimer1Started == pdPASS) && (xTimer2Started == pdPASS)) {
            printf("Timers started successfully\n");
        } else {
            printf("Failed to start timers\n");
        }
    } else {
        printf("Failed to create timers\n");
    }
    for( ; ; )
    {
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    // Giữ chương trình không thoát khỏi app_main
    // while (1) {
    //     // vTaskDelay(pdMS_TO_TICKS(100));
    // }

    return 0;
}
// int app_main( void )
// {
//     TimerHandle_t xAutoReloadTimer, xOneShotTimer;
//     BaseType_t xTimer1Started, xTimer2Started;
//     xOneShotTimer = xTimerCreate( "OneShot",                 /* Text name for the software timer - not used by FreeRTOS. */
//                                   mainONE_SHOT_TIMER_PERIOD, /* The software timer's period in ticks. */
//                                   pdFALSE,                   /* Setting uxAutoRealod to pdFALSE creates a one-shot software timer. */
//                                   0,                         /* This example does not use the timer id. */
//                                   prvOneShotTimerCallback ); /* The callback function to be used by the software timer being created. */

//     xAutoReloadTimer = xTimerCreate( "AutoReload",                 /* Text name for the software timer - not used by FreeRTOS. */
//                                      mainAUTO_RELOAD_TIMER_PERIOD, /* The software timer's period in ticks. */
//                                      pdTRUE,                       /* Set uxAutoRealod to pdTRUE to create an auto-reload software timer. */
//                                      0,                            /* This example does not use the timer id. */
//                                      prvAutoReloadTimerCallback ); /* The callback function to be used by the software timer being created. */

//     if( ( xOneShotTimer != NULL ) && ( xAutoReloadTimer != NULL ) )
//     {
//         xTimer1Started = xTimerStart( xOneShotTimer, 0 );
//         xTimer2Started = xTimerStart( xAutoReloadTimer, 0 );
//         if( ( xTimer1Started == pdPASS ) && ( xTimer2Started == pdPASS ) )
//         {
//             /* Start the scheduler. */
//             vTaskStartScheduler();
//         }
//     }

//     for( ; ; )
//     {
//     }

//     return 0;
// }
/*-----------------------------------------------------------*/

static void prvOneShotTimerCallback( TimerHandle_t xTimer )
{
    static TickType_t xTimeNow;

    /* Obtain the current tick count. */
    xTimeNow = xTaskGetTickCount();

    /* Output a string to show the time at which the callback was executed. */
    printf( "One-shot timer callback executing %ld\n", xTimeNow );
}
/*-----------------------------------------------------------*/

static void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{
    static TickType_t xTimeNow;

    /* Obtain the current tick count. */
    xTimeNow = xTaskGetTickCount();

    /* Output a string to show the time at which the callback was executed. */
    printf( "Auto-reload timer callback executing %ld\n", xTimeNow );
}
/*-----------------------------------------------------------*/
