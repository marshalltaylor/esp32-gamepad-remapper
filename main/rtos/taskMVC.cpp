/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- FreeRTOS system -----------------------------------------------*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"

#include "os.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
#include "taskSystem.h"
#include "globals.h"
#include "commands.h"

/* Includes -- modules -------------------------------------------------------*/
// #include "logging.h"

/* References ----------------------------------------------------------------*/
// #define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
Logging mvcDebug;
// #define localPrintf consoleDebug.printf

// Send output to two devices
static void
localPrintf (const char *fmt, ...)
{
    // Setup
    va_list args;
    va_start(args, fmt);
    // Send to serial
    if (mvcDebug.pVPrintf)
    {
        mvcDebug.pVPrintf(fmt, args);
    }
    va_end(args);
}

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

/* Functions -----------------------------------------------------------------*/
extern "C" void
taskMVCStart (void *argument)
{
#ifdef USE_LOGGING
    mvcDebug.setStamp("MVC", 3);
    mvcDebug.setMode(LOG_MODE_DEFAULT);
#endif
    localPrintf("MVC task started.\n");
    while (1)
    {
        // localPrintf("MVC wait for bits\n");
        EventBits_t uxBits = xEventGroupGetBits(xTestEventGroup);
        // localPrintf("MVC check bits\n");
        if (!(uxBits & 0x08))
        {
            // localPrintf("MVC tick tock\n");
            controller.tick(10);
        }
        // localPrintf("MVC spinski\n");
        vTaskDelay(pdMS_TO_TICKS(10));
        // Slow rate and toggle LED at a low level, for test
        // bspIOPinWrite(LED_1, bspIOPinRead(LED_1) ^ 1u);
        // vTaskDelay(pdMS_TO_TICKS(500));
    }
}
