//Alunos Fabricio Guite Pereira e Luiz Guilherme Durau Rodrigues
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "basic_io.h"
#include "semphr.h"
#include "time.h"

void vTask1(void *pvParameters);
void vTask2(void* pvParameters);
void vTask3(void* pvParameters);
SemaphoreHandle_t xMutex;
char display[50];

void vTask1(void *pvParameters) {
    int idtarefa = *(int *)pvParameters;
    time_t segundos;
    struct tm* datetimeNow;
    int dia, mes, ano;
    
    for (;;) {
        // Obtendo o tempo em segundos
        time(&segundos);
        // Converter unidade segundos para tempo local
        datetimeNow = localtime(&segundos);
        // Obtendo o dia
        dia = datetimeNow->tm_mday;
        // Obtendo o mes
        mes = datetimeNow->tm_mon + 1;
        // Obtendo o ano
        ano = datetimeNow->tm_year + 1900;
        
        xSemaphoreTake(xMutex, portMAX_DELAY);
        snprintf(display, 50, "task %d - Data: %02d/%02d/%04d ", idtarefa, dia, mes, ano);
        xSemaphoreGive(xMutex);
        
        vPrintString(display);
        vTaskDelay(1000);
    }
    vTaskDelete(NULL);
}

void vTask2(void* pvParameters) {
    int idtarefa = *(int *)pvParameters;
    time_t segundos;
    struct tm* datetimeNow;
    int hour, min, sec;
    
    for (;;) {
        // Obtendo o tempo em segundos
        time(&segundos);
        // Converter unidade segundos para tempo local
        datetimeNow = localtime(&segundos);
        // Obtendo a hora
        hour = datetimeNow->tm_hour;
        // Obtendo os minutos
        min = datetimeNow->tm_min;
        // Obtendo segundos
        sec = datetimeNow->tm_sec;
        
        xSemaphoreTake(xMutex, portMAX_DELAY);
        snprintf(display, 50, "task %d - Hora: %02d:%02d:%02d ", idtarefa, hour, min, sec);
        xSemaphoreGive(xMutex);
        
        vPrintString(display);
        vTaskDelay(1000);
    }
    vTaskDelete(NULL);
}

void vTask3(void* pvParameters) {
    int idtarefa = *(int *)pvParameters;
    const float maxTemp = 40.0;
    float temp;
    
    for (;;) {
        // Obtendo um número float aleatório, valor máximo definido na constante maxTemp
        temp = (float)rand() / (float)RAND_MAX * maxTemp;
        
        xSemaphoreTake(xMutex, portMAX_DELAY);
        // Adiciona conteúdo na variável display
        snprintf(display, 50, "task %d - Curitiba %.2f C\n", idtarefa, temp);
        xSemaphoreGive(xMutex);
        
        // Imprime o conteúdo do display na saída do console
        vPrintString(display);
        vTaskDelay(1000);
    }
    vTaskDelete(NULL);
}

int main_(void) {
    xMutex = xSemaphoreCreateMutex();
    xTaskCreate(vTask1, "vTask1", configMINIMAL_STACK_SIZE, &(int){1}, 1, NULL);
    xTaskCreate(vTask2, "vTask2", configMINIMAL_STACK_SIZE, &(int){2}, 1, NULL);
    xTaskCreate(vTask3, "vTask3", configMINIMAL_STACK_SIZE, &(int){3}, 1, NULL);
    vTaskStartScheduler();
    
    for (;;) {
    }
    return 0;
}
