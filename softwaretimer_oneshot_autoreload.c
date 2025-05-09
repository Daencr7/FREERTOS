#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/timers.h"

#define GPIO_OUT GPIO_NUM_2
#define HIGH 1
#define LOW 0
#define mainONE_SHOT_TIMER_PERIOD pdMS_TO_TICKS( 3333UL )
#define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS( 500UL )
// static void prvOneShotTimerCallback( TimerHandle_t xTimer);
// static void prvAutoReloadTimerCallback( TimerHandle_t xTimer);
static void prvTimerCallback( TimerHandle_t xTimer);
static const char *TAG = "ESP";
// static void vTask1( void * pvParameters);
// static void vTask2( void * pvParameters);
// void led_init(void);
TimerHandle_t xAutoReloadTimer, xOneShotTimer;
int app_main() {
    // TimerHandle_t xAutoReloadTimer, xOneShotTimer;
    BaseType_t xTimer1Started, xTimer2Started;
    
    xOneShotTimer = xTimerCreate("OneShot", 
                                mainONE_SHOT_TIMER_PERIOD,
                                pdFALSE,
                                NULL,
                                prvTimerCallback );


    xAutoReloadTimer = xTimerCreate("AutoReload", 
                                mainAUTO_RELOAD_TIMER_PERIOD,
                                pdTRUE,
                                NULL,
                                prvTimerCallback );
    if( (xOneShotTimer != NULL ) && ( xAutoReloadTimer != NULL ) ){
        if( ( xAutoReloadTimer != NULL ) ){
            xTimer1Started = xTimerStart( xOneShotTimer, 0);
            xTimer2Started = xTimerStart( xAutoReloadTimer, 0);
            // if( (xTimer1Started == pdPASS) && (xTimer2Started == pdPASS)){
            //     vTaskStartScheduler();
            // }
        }
    }
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    return 0;
}
static void prvTimerCallback( TimerHandle_t xTimer){
    TickType_t xTimeNow;
    uint32_t ulExecutionCount;

    ulExecutionCount = (uint32_t) pvTimerGetTimerID( xTimer );
    ulExecutionCount++;
    vTimerSetTimerID( xTimer, (void*) ulExecutionCount );

    xTimeNow = xTaskGetTickCount();

    if( xTimer == xOneShotTimer ){
        printf("One-shot timer callback executing %ld\n", xTimeNow);
    }
    else{
        printf("Auto-reload timer callback executing %ld\n", xTimeNow);

        if(ulExecutionCount == 5){
            xTimerStop( xTimer, 0 );
        }
    }
}

// void led_init(void) {
//     gpio_config_t io_conf = {
//         .pin_bit_mask = 1ULL << GPIO_OUT,
//         .mode = GPIO_MODE_OUTPUT,
//         .pull_up_en = 0,
//         .pull_down_en = 0,
//         .intr_type = GPIO_INTR_DISABLE
//     };
//     gpio_config(&io_conf);
// }

// static void prvOneShotTimerCallback( TimerHandle_t xTimer){
//     TickType_t xTimeNow;
//     xTimeNow = xTaskGetTickCount();
//     printf( " One-shot timer callback executing %ld\n", xTimeNow );

//     // ulCallCount++;

// }
// 
// static void prvAutoReloadTimerCallback( TimerHandle_t xTimer){
//     TickType_t xTimeNow;
//     xTimeNow = xTaskGetTickCount();
//     gpio_set_level(GPIO_NUM_2, 1);
//     vTaskDelay(pdMS_TO_TICKS(50));
//     gpio_set_level(GPIO_NUM_2, 0);
//     printf(" Auto-reload timer callback executing %ld\n", xTimeNow);
//     // ulCallCount++;
// }
// static void prvStdioGatekeeperTask( void *pvParameters){
//     char *pcMessageToPrint;
//     for(;;){
//         xQueueReceive( xPrintQueue, &pcMessageToPrint, portMAX_DELAY );
//         printf("%s", pcMessageToPrint);
//         fflush( stdout );
//     }

// }
// static void prvPrintTask( void *pvParameters){
//     int iIndexToString;
//     const TickType_t xMaxBlockTimeTicks = 0x20;
//     iIndexToString = (int) pvParameters;
//     for(;;){
//         xQueueSendToBack (xPrintQueue, &(pcStringToPrint[iIndexToString]), 0);
//         vTaskDelay( (rand() % xMaxBlockTimeTicks ) );
//     }
// }

// void vApplicationTickHook( void ){
//     static int iCount = 0;
//     iCount++;
//     if( iCount >= 200){
//         xQueueSendToFrontFromISR( xPrintQueue, &( pcStringToPrint[ 2 ]), NULL);
//         iCount = 0;
//     }
// }


    // TimerHandle_t xAutoReloadTimer, xOneShotTimer;
    // BaseType_t xTimer1Started, xTimer2Started;
    
    // xOneShotTimer = xTimerCreate("OneShot", 
    //                             mainONE_SHOT_TIMER_PERIOD,
    //                             pdFALSE,
    //                             0,
    //                             prvOneShotTimerCallback );


    // xAutoReloadTimer = xTimerCreate("AutoReload", 
    //                             mainAUTO_RELOAD_TIMER_PERIOD,
    //                             pdTRUE,
    //                             0,
    //                             prvAutoReloadTimerCallback );
    // if( (xOneShotTimer != NULL ) && ( xAutoReloadTimer != NULL ) ){
    // if( ( xAutoReloadTimer != NULL ) ){
        // xTimer1Started = xTimerStart( xOneShotTimer, 0);
        // xTimer2Started = xTimerStart( xAutoReloadTimer, 0);
        // if( (xTimer1Started == pdPASS) && (xTimer2Started == pdPASS)){
        //     vTaskStartScheduler();
        // }

// SemaphoreHandle_t xMutex;
// static char *pcStringToPrint[] = {
//     "Task 1 ***********************************************\r\n",
//     "Task 2 -----------------------------------------------\r\n",
//     "Message printed from the tick hook interrupt #########\r\n"
// };
// QueueHandle_t xPrintQueue;
// static void prvStdioGatekeeperTask( void *pvParameters);
// static void prvPrintTask( void *pvParameters);
// void vApplicationTickHook( void )
// #define mainONE_SHOT_TIMER_PERIOD pdMS_TO_TICKS( 3333 )
// #define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS( 200 )
