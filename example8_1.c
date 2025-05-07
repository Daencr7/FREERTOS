#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

// static void vSenderTask(void * pvParameter);
// static void vReceiverTask(void * pvParameter);

// QueueHandle_t xQueue;
SemaphoreHandle_t xMutex;
// static int32_t value1 = 100;
// static int32_t value2 = 200;
// typedef enum{
//     eSender1,
//     eSender2
// } DataSource_t;

// typedef struct{
//     uint8_t ucValue;
//     DataSource_t eDataSource;
// } Data_t;
// static const Data_t xStructsToSend[ 2 ] = {
//     {100, eSender1},
//     {200, eSender2}
// }; 
static void prvNewPrintString( const char *pcString);
static void prvPrintTask( void *pvParameter);
int app_main() {
    xMutex = xSemaphoreCreateMutex();
    if(xMutex != NULL){
        xTaskCreate(prvPrintTask, "Print1", 2048,
                    "Task1 *******************\r\n", 1, NULL);
        xTaskCreate(prvPrintTask, "Print2", 2048,
                    "Task2 -------------------\r\n", 2, NULL);
    }
    // xQueue = xQueueCreate(3, sizeof(Data_t));
    // if (xQueue != NULL) {
    //     xTaskCreate( vSenderTask, "Sender1", 2048, &( xStructsToSend[ 0 ] ), 2, NULL ); 
    //     xTaskCreate( vSenderTask, "Sender2", 2048, &( xStructsToSend[ 1 ] ), 2, NULL ); 
    //     xTaskCreate(vReceiverTask, "Receiver", 4096, NULL, 1, NULL);
    // }
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    return 0;
}
static void prvNewPrintString (const char *pcString){
    xSemaphoreTake( xMutex, portMAX_DELAY );
    {
        printf("%s",pcString);
        fflush(stdout);
    }
    xSemaphoreGive( xMutex );
}
static void prvPrintTask( void *pvParameter){
    char *pcStringToPrint;
    const TickType_t xMaxBlockTimeTicks = 0x20;
    pcStringToPrint = (char *) pvParameter;
    for(;;){
        prvNewPrintString(pcStringToPrint);
        vTaskDelay((rand()% xMaxBlockTimeTicks));
    }
}
// static void vSenderTask(void * pvParameters) {
//     BaseType_t xStatus; 
//     const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 ); 
//     for( ;; ){  
//             xStatus = xQueueSendToBack( xQueue, pvParameters, xTicksToWait ); 
//             if( xStatus != pdPASS ) 
//             {  
//                 printf( "Could not send to the queue.\r\n" ); 
//             } 
//             // vTaskDelay(pdMS_TO_TICKS(100));
//         } 
// }
// static void vReceiverTask( void *pvParameters )
// { 
//    Data_t xReceivedStructure; 
//    BaseType_t xStatus; 
//     for( ;; ) { 
//         if( uxQueueMessagesWaiting( xQueue ) != 3 ) 
//         { 
//             printf( "Queue should have been full!\r\n" ); 
//         } 

//         xStatus = xQueueReceive( xQueue, &xReceivedStructure, 0 ); 

//         if( xStatus == pdPASS ) 
//         { 
//             if( xReceivedStructure.eDataSource == eSender1 ) 
//             { 
//                 printf( "From Sender 1 = %d \n", xReceivedStructure.ucValue ); 
//             } 
//             else 
//             { 
//                 printf( "From Sender 2 = %d \n", xReceivedStructure.ucValue ); 
//             } 
//         } 
//         else 
//         { 
//             printf( "Could not receive from the queue.\r\n" ); 
//         } 
//         // vTaskDelay(pdMS_TO_TICKS(100));
//     } 
// }
