
#include "sdkconfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"

#include "bsp.h"
#include "os.h"
#include "globals.h"

#include "commands.h"

#include "taskLog.h"

// Connect directly to bsp.
#define localPrintf bspPrintf

TaskStatus_t pxTaskStatusArray[10];

int statProgram(int argc, char *argv[]);
// int bitProgram(int argc, char *argv[]);
// int delayProgram(int argc, char *argv[]);
// int helloProgram(int argc, char *argv[]);
// int xqProgram(int argc, char *argv[]);

commandInfo_t statCmd = { "stat",
                          "CPU usage",
                          "No arguments.  Provides some info about the rtos.",
                          &statProgram };

// commandInfo_t bitCmd = {
//     "bit",
//     "Set or clear general control bits",
//     " usage: bit [set|clr] <bit num>",
//     &bitProgram
// };
//
// commandInfo_t delayCmd = {
//     "delay",
//     "Load or sleep task",
//     " usage: delay [task|load|forever]\n Caution! forever enters a while(1)
//     loop", &delayProgram
// };
//
// commandInfo_t helloCmd = {
//     "hello",
//     "Send message over q",
//     "TODO",
//     &helloProgram
// };
//
// commandInfo_t xqCmd = {
//     "xq",
//     "Send message over q",
//     "TODO",
//     &xqProgram
// };

#define ARRAY_SIZE_OFFSET \
    5 // Increase this if print_real_time_stats returns ESP_ERR_INVALID_SIZE

/**
 * @brief   Function to print the CPU usage of tasks over a given duration.
 *
 * This function will measure and print the CPU usage of tasks over a specified
 * number of ticks (i.e. real time stats). This is implemented by simply calling
 * uxTaskGetSystemState() twice separated by a delay, then calculating the
 * differences of task run times before and after the delay.
 *
 * @note    If any tasks are added or removed during the delay, the stats of
 *          those tasks will not be printed.
 * @note    This function should be called from a high priority task to minimize
 *          inaccuracies with delays.
 * @note    When running in dual core mode, each core will correspond to 50% of
 *          the run time.
 *
 * @param   xTicksToWait    Period of stats measurement
 *
 * @return
 *  - ESP_OK                Success
 *  - ESP_ERR_NO_MEM        Insufficient memory to allocated internal arrays
 *  - ESP_ERR_INVALID_SIZE  Insufficient array size for uxTaskGetSystemState.
 * Trying increasing ARRAY_SIZE_OFFSET
 *  - ESP_ERR_INVALID_STATE Delay duration too short
 */
static esp_err_t
print_real_time_stats (TickType_t xTicksToWait)
{
    uint32_t                    total_elapsed_time = 0;
    TaskStatus_t               *start_array = NULL, *end_array = NULL;
    UBaseType_t                 start_array_size, end_array_size;
    configRUN_TIME_COUNTER_TYPE start_run_time, end_run_time;
    esp_err_t                   ret;

    // Allocate array to store current task states
    start_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    start_array
        = (TaskStatus_t *)malloc(sizeof(TaskStatus_t) * start_array_size);
    if (start_array == NULL)
    {
        ret = ESP_ERR_NO_MEM;
        goto exit;
    }
    // Get current task states
    start_array_size
        = uxTaskGetSystemState(start_array, start_array_size, &start_run_time);
    if (start_array_size == 0)
    {
        ret = ESP_ERR_INVALID_SIZE;
        goto exit;
    }

    vTaskDelay(xTicksToWait);

    // Allocate array to store tasks states post delay
    end_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    end_array = (TaskStatus_t *)malloc(sizeof(TaskStatus_t) * end_array_size);
    if (end_array == NULL)
    {
        ret = ESP_ERR_NO_MEM;
        goto exit;
    }
    // Get post delay task states
    end_array_size
        = uxTaskGetSystemState(end_array, end_array_size, &end_run_time);
    if (end_array_size == 0)
    {
        ret = ESP_ERR_INVALID_SIZE;
        goto exit;
    }

    // Calculate total_elapsed_time in units of run time stats clock period.
    total_elapsed_time = (end_run_time - start_run_time);
    if (total_elapsed_time == 0)
    {
        ret = ESP_ERR_INVALID_STATE;
        goto exit;
    }

    localPrintf("| Task | Run Time | Percentage\n");
    // Match each task in start_array to those in the end_array
    for (int i = 0; i < start_array_size; i++)
    {
        int k = -1;
        for (int j = 0; j < end_array_size; j++)
        {
            if (start_array[i].xHandle == end_array[j].xHandle)
            {
                k = j;
                // Mark that task have been matched by overwriting their handles
                start_array[i].xHandle = NULL;
                end_array[j].xHandle   = NULL;
                break;
            }
        }
        // Check if matching task found
        if (k >= 0)
        {
            uint32_t task_elapsed_time = end_array[k].ulRunTimeCounter
                                         - start_array[i].ulRunTimeCounter;
            uint32_t percentage_time
                = (task_elapsed_time * 100UL)
                  / (total_elapsed_time * CONFIG_FREERTOS_NUMBER_OF_CORES);
            localPrintf("| %s | %" PRIu32 " | %" PRIu32 "%%\n",
                        start_array[i].pcTaskName,
                        task_elapsed_time,
                        percentage_time);
        }
    }

    // Print unmatched tasks
    for (int i = 0; i < start_array_size; i++)
    {
        if (start_array[i].xHandle != NULL)
        {
            localPrintf("| %s | Deleted\n", start_array[i].pcTaskName);
        }
    }
    for (int i = 0; i < end_array_size; i++)
    {
        if (end_array[i].xHandle != NULL)
        {
            localPrintf("| %s | Created\n", end_array[i].pcTaskName);
        }
    }
    ret = ESP_OK;

exit: // Common return path
    free(start_array);
    free(end_array);
    return ret;
}

int
statProgram (int argc, char *argv[])
{
    localPrintf("\nGetting real time stats over %" PRIu32 " ticks\n",
                pdMS_TO_TICKS(1000));
    if (print_real_time_stats(pdMS_TO_TICKS(1000)) == ESP_OK)
    {
        localPrintf("Real time stats obtained\n");
    }
    else
    {
        localPrintf("Error getting real time stats\n");
    }
    return 0;
}

// int bitProgram(int argc, char *argv[])
//{
//     EventBits_t uxBits = xEventGroupGetBits(xTestEventGroup);
//     if(argc < 2)
//     {
//         localPrintf("needs arg and value\n");
//         return 1;
//     }
//     int intArg = atoi(argv[1]);
//     if((intArg < 0)||(intArg > 7))
//     {
//         localPrintf("value 0 - 7\n");
//     }
//     else
//     {
//         uint16_t testKeyMask = 0;
//         testKeyMask = 0x0001 << intArg;
//         if(0 == strcmp((const char*)argv[0], "set"))
//         {
//             uxBits |= testKeyMask;
//             xEventGroupSetBits(xTestEventGroup, testKeyMask );
//         }
//         else if(0 == strcmp((const char*)argv[0], "clr"))
//         {
//             uxBits &= ~testKeyMask;
//             xEventGroupClearBits(xTestEventGroup, testKeyMask );
//         }
//         else
//         {
//             localPrintf("Needs 'set' or 'clr'\n");
//         }
//     }
//     for(int i = 7; i >= 0; i--)
//     {
//         localPrintf("%d", (uxBits >> i)&0x0001);
//     }
//     return 0;
// }
//
// int delayProgram(int argc, char *argv[])
//{
//     if(0 == strcmp((const char*)argv[0], "task"))
//     {
//         //Test delay times
//         localPrintf("\n");
//         localPrintf("Start time  %dms\n", millis());
//         vTaskDelay( 1000 );
//         localPrintf("Stop time   %dms\n", millis());
//         //localPrintf("tick count     %dms\n", xTaskGetTickCount());
//     }
//     else if(0 == strcmp((const char*)argv[0], "load"))
//     {
//         //Test delay times
//         localPrintf("L");
//         bspDelay( 333 );
//         localPrintf("O");
//         bspDelay( 333 );
//         localPrintf("A");
//         bspDelay( 333 );
//         localPrintf("D");
//     }
//     else if(0 == strcmp((const char*)argv[0], "forever"))
//     {
//         while(1);
//     }
//     else
//     {
//         localPrintf("Error: args\n");
//     }
//     return 0;
// }
//
// int helloProgram(int argc, char *argv[])
//{
//     strMsg_t * msg = new strMsg_t();
//
//     msg->id = 0;
//     sprintf( msg->data, "Hello world!\n");
//     if(pdPASS != xQueueSend( logQueue, &msg, 0 ))
//     {
//         //TODO: error on send
//         delete msg;
//     }
//     return 0;
// }
//
// int xqProgram(int argc, char *argv[])
//{
//     localPrintf("3");
//     bspDelay(300);
//     localPrintf("2");
//     bspDelay(300);
//     localPrintf("1");
//     bspDelay(300);
//
//     strMsg_t * msg = new strMsg_t();
//
//     msg->id = -1;
//     msg->data[0] = '\0';
//     if(pdPASS != xQueueSend( logQueue, &msg, 0 ))
//     {
//         localPrintf(".dud");
//         delete msg;
//     }
//     return 0;
// }
//