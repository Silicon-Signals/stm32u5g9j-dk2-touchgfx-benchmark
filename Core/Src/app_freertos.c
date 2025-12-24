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

//Result screen variable
volatile uint32_t g_fps = 0;
volatile uint32_t g_render_time = 0;
volatile uint32_t g_cpu_usage = 0;
volatile uint32_t g_stack_usage = 0;
volatile uint32_t g_heap_usage = 0;

//Demo sampling variables
volatile bool demo_running = false;
volatile bool demo_complete = false;
volatile uint32_t sample_count = 0;
#define NUM_SAMPLES 10
uint32_t fps_samples[NUM_SAMPLES];
uint32_t render_time_samples[NUM_SAMPLES];
uint32_t cpu_usage_samples[NUM_SAMPLES];
uint32_t stack_usage_samples[NUM_SAMPLES];
uint32_t heap_usage_samples[NUM_SAMPLES];

// NEW: Average values
volatile uint32_t avg_fps = 0;
volatile uint32_t avg_render_time = 0;
volatile uint32_t avg_cpu_usage = 0;
volatile uint32_t avg_stack_usage = 0;
volatile uint32_t avg_heap_usage = 0;
volatile uint32_t test_name = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for GUI_Task */
osThreadId_t GUI_TaskHandle;
const osThreadAttr_t GUI_Task_attributes = {
  .name = "GUI_Task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 8192 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern portBASE_TYPE IdleTaskHook(void* p);

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
void vApplicationIdleHook( void )
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
//	  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
//	  DWT->CYCCNT = 0;  // Optional reset, but not required
//	  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	 __HAL_RCC_TIM2_CLK_ENABLE();

	    htim2.Instance = TIM2;
	    htim2.Init.Prescaler = 40; // 4 MHz tick
	    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	    htim2.Init.Period = 0xFFFFFFFF;  // Free running 32-bit counter
	    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	    HAL_TIM_Base_Init(&htim2);
	    HAL_TIM_Base_Start(&htim2);
}

__weak unsigned long getRunTimeCounterValue(void)
{
return 	__HAL_TIM_GET_COUNTER(&htim2);		//__HAL_TIM_GET_COUNTER(&htim2);
}

/* USER CODE END 2 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
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
  for(;;)
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

    if (pxTaskStatusArray != NULL) {
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

void metrics_print(void) {
    static uint32_t last_time = 0;
    static uint32_t last_frames = 0;

    uint32_t current_ms = HAL_GetTick();
    uint32_t elapsed_ms = current_ms - last_time;
    if (elapsed_ms >= 1000) {
        last_time = current_ms;
        g_stack_usage = (GUI_Task_attributes.stack_size - uxTaskGetStackHighWaterMark(GUI_TaskHandle)) / 1024; // In KB
        g_heap_usage = (configTOTAL_HEAP_SIZE - xPortGetFreeHeapSize()) / 1024; // In KB
        g_cpu_usage = GetTaskCPUUsage(GUI_TaskHandle); // In percentage
        uint32_t frames = (frame_counter - last_frames);
        g_fps = frames > 60 ? 60 : frames; // Cap at 60 FPS
        g_render_time = render_time; // In ms
        last_frames = frame_counter;

        // Collect samples if demo is running
        if (demo_running)
        {
            if (sample_count < NUM_SAMPLES)
            {
                fps_samples[sample_count] = g_fps;
                render_time_samples[sample_count] = g_render_time;
                cpu_usage_samples[sample_count] = g_cpu_usage;
                stack_usage_samples[sample_count] = g_stack_usage;
                heap_usage_samples[sample_count] = g_heap_usage;
                sample_count++;
            }

            // After 10 samples, calculate averages and complete demo
            if (sample_count >= NUM_SAMPLES)
            {
                // Calculate averages
                uint32_t sum_fps = 0, sum_render_time = 0, sum_cpu_usage = 0, sum_stack_usage = 0, sum_heap_usage = 0;
                for (uint32_t i = 0; i < NUM_SAMPLES; i++)
                {
                    sum_fps += fps_samples[i];
                    sum_render_time += render_time_samples[i];
                    sum_cpu_usage += cpu_usage_samples[i];
                    sum_stack_usage += stack_usage_samples[i];
                    sum_heap_usage += heap_usage_samples[i];
                }
                avg_fps = sum_fps / NUM_SAMPLES;
                avg_render_time = sum_render_time / NUM_SAMPLES;
                avg_cpu_usage = sum_cpu_usage / NUM_SAMPLES;
                avg_stack_usage = sum_stack_usage / NUM_SAMPLES;
                avg_heap_usage = sum_heap_usage / NUM_SAMPLES;

                demo_running = false;
                demo_complete = true;
                sample_count = 0;
            }
        }
    }
}

/* USER CODE END Application */

