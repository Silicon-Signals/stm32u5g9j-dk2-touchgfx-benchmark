/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os2.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
/* USER CODE BEGIN Variables */
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;
float percentage = 0;
extern volatile uint32_t frame_counter;
UBaseType_t StackUsage;
// Render Time variables
extern volatile uint32_t current_ms;
extern volatile uint32_t render_time;
extern uint32_t render_start_ms;
extern uint32_t render_end_ms;

// Result screen variable
volatile uint32_t g_fps = 0;
volatile uint32_t g_render_time = 0;
volatile uint32_t g_cpu_usage = 0;
volatile uint32_t g_ram_usage = 0;
volatile uint32_t g_internal_flash = 0;
volatile uint32_t g_external_flash = 0;

// Demo sampling variables
volatile bool demo_running = false;
volatile bool demo_complete = false;
volatile uint32_t sample_count = 0;
#define NUM_SAMPLES 10
uint32_t render_time_samples[NUM_SAMPLES];
uint32_t cpu_usage_samples[NUM_SAMPLES];
uint32_t ram_usage_samples[NUM_SAMPLES];
uint32_t internal_flash_samples[NUM_SAMPLES];
uint32_t external_flash_samples[NUM_SAMPLES];

//RAM and Flash usage variables
extern uint32_t __framebuffer_start__;
extern uint32_t __framebuffer_end__;
extern uint32_t __videobuffer_start__;
extern uint32_t __videobuffer_end__;
extern uint32_t _edata;
extern uint32_t _sdata;
extern uint32_t _ebss;
extern uint32_t _sbss;
extern uint32_t _sidata;
extern uint32_t __ext_flash_end;
extern uint32_t __ext_flash_start;
extern uint32_t nema_stencil_buffer_size;

// NEW: Average values
volatile uint32_t avg_render_time = 0;
volatile uint32_t avg_cpu_usage = 0;
volatile uint32_t avg_ram_usage = 0;
volatile uint32_t avg_internal_flash = 0;
volatile uint32_t avg_external_flash = 0;
volatile uint32_t test_name = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 1024 * 4};
/* Definitions for GUI_Task */
osThreadId_t GUI_TaskHandle;
const osThreadAttr_t GUI_Task_attributes = {
    .name = "GUI_Task",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 8192 * 4};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern portBASE_TYPE IdleTaskHook(void *p);

void metrics_print(void);
int GetTaskCPUUsage(osThreadId_t thread_id);
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void TouchGFX_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook(void)
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
    task. It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()). If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */

    vTaskSetApplicationTaskTag(NULL, IdleTaskHook);
}

__weak void configureTimerForRunTimeStats(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 40; // 4 MHz tick
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFFFFFF; // Free running 32-bit counter
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&htim2);
    HAL_TIM_Base_Start(&htim2);
}

__weak unsigned long getRunTimeCounterValue(void)
{
    return __HAL_TIM_GET_COUNTER(&htim2); //__HAL_TIM_GET_COUNTER(&htim2);
}

/* USER CODE END 2 */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* creation of GUI_Task */
    GUI_TaskHandle = osThreadNew(TouchGFX_Task, NULL, &GUI_Task_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief Function implementing the defaultTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    /* USER CODE BEGIN defaultTask */
    /* Infinite loop */
    for (;;)
    {
        metrics_print();
        osDelay(1);
    }
    /* USER CODE END defaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

int GetTaskCPUUsage(osThreadId_t thread_id)
{
    static uint32_t lastTotalRunTime = 0;
    static uint32_t lastTaskRunTime = 0;

    UBaseType_t taskCount;
    TaskStatus_t *pxTaskStatusArray;
    uint32_t totalRunTime;
    int result = 0;

    taskCount = uxTaskGetNumberOfTasks();
    pxTaskStatusArray = pvPortMalloc(taskCount * sizeof(TaskStatus_t));

    if (pxTaskStatusArray != NULL)
    {
        taskCount = uxTaskGetSystemState(pxTaskStatusArray, taskCount, &totalRunTime);
        uint32_t deltaTotal = totalRunTime - lastTotalRunTime;

        if (deltaTotal > 0)
        {
            for (UBaseType_t i = 0; i < taskCount; i++)
            {
                if (pxTaskStatusArray[i].xHandle == (TaskHandle_t)thread_id)
                {
                    uint32_t deltaTask = pxTaskStatusArray[i].ulRunTimeCounter - lastTaskRunTime;
                    percentage = ((float)deltaTask / (float)deltaTotal) * 100.0f;
                    result = (int)percentage;

                    lastTaskRunTime = pxTaskStatusArray[i].ulRunTimeCounter;
                    break;
                }
            }
        }
        lastTotalRunTime = totalRunTime;
        vPortFree(pxTaskStatusArray);
    }
    return result;
}

void metrics_print(void)
{
    static uint32_t last_time = 0;
    static uint32_t last_frames = 0;

    uint32_t current_ms = HAL_GetTick();
    uint32_t elapsed_ms = current_ms - last_time;

    uint32_t flash_base = 0x08000000;
	uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
	uint32_t total_internal_flash_usage = ((uint32_t)&_sidata - flash_base) + data_size;

	uint32_t total_ext_flash_usage = (uint32_t)&__ext_flash_end - (uint32_t)&__ext_flash_start;

    if (elapsed_ms >= 1000)
    {
        last_time = current_ms;
        g_cpu_usage = GetTaskCPUUsage(GUI_TaskHandle);
        uint32_t frames = (frame_counter - last_frames);
        g_fps = frames > 60 ? 60 : frames;
        g_render_time = render_time;
        last_frames = frame_counter;
        g_ram_usage = (((uint32_t)&_edata - (uint32_t)&_sdata) +
   			 ((uint32_t)&_ebss - (uint32_t)&_sbss) +
   			 ((uint32_t)&__framebuffer_end__ - (uint32_t)&__framebuffer_start__) +
			 ((uint32_t)&__videobuffer_end__ - (uint32_t)&__videobuffer_start__)) / 1024;
        g_internal_flash = total_internal_flash_usage / 1024 ;
        g_external_flash = total_ext_flash_usage / 1048576 ;

        // Collect samples if demo is running
        if (demo_running)
        {
            if (sample_count < NUM_SAMPLES)
            {
                render_time_samples[sample_count] = g_render_time;
                cpu_usage_samples[sample_count] = g_cpu_usage;
                ram_usage_samples[sample_count] = g_ram_usage;
                internal_flash_samples[sample_count] = g_internal_flash;
                external_flash_samples[sample_count] = g_external_flash;
                sample_count++;
            }

            // After 10 samples, calculate averages and complete demo
            if (sample_count >= NUM_SAMPLES)
            {
                // Calculate averages
                uint32_t sum_render_time = 0, sum_cpu_usage = 0, sum_ram_usage = 0, sum_internal_flash = 0,  sum_external_flash = 0;
                for (uint32_t i = 0; i < NUM_SAMPLES; i++)
                {
                    sum_render_time += render_time_samples[i];
                    sum_cpu_usage += cpu_usage_samples[i];
                    sum_ram_usage += ram_usage_samples[i];
                    sum_internal_flash += internal_flash_samples[i];
                    sum_external_flash += external_flash_samples[i];
                }
                avg_render_time = sum_render_time / NUM_SAMPLES;
                avg_cpu_usage = sum_cpu_usage / NUM_SAMPLES;
                avg_ram_usage = sum_ram_usage / NUM_SAMPLES;
                avg_internal_flash = sum_internal_flash / NUM_SAMPLES;
                avg_external_flash = sum_external_flash / NUM_SAMPLES;

                demo_running = false;
                demo_complete = true;
                sample_count = 0;

            }
        }
    }
}

/* USER CODE END Application */
