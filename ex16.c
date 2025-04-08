#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xBinarySemaphore;

void vHandlerTask(void* pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY)) {
            printf("Handler task - Processing event.\n");
        }
    }
}

void vPeriodicTask(void* pvParameters)
{
    const TickType_t xDelay500ms = pdMS_TO_TICKS(500);
    while (1)
    {
        vTaskDelay(xDelay500ms);
        printf("Periodic task - About to simulate interrupt.\n");
        xSemaphoreGive(xBinarySemaphore);  // Không cần ISR
        printf("Periodic task - Simulated interrupt sent.\n\n");
    }
}

void app_main(void)
{
    xBinarySemaphore = xSemaphoreCreateBinary();

    if (xBinarySemaphore != NULL)
    {
        xTaskCreate(vHandlerTask, "Handler", 2048, NULL, 3, NULL);
        xTaskCreate(vPeriodicTask, "Periodic", 2048, NULL, 1, NULL);
        printf("Tasks created. System running.\n");
    }
    else
    {
        printf("Failed to create semaphore.\n");
    }
}
