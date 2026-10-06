/* FreeRTOS Real Time Stats Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

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

void
app_main (void)
{
    // Allow other core to finish initialization
    vTaskDelay(pdMS_TO_TICKS(100));

    printf("START\n");
    bspIOPinInit(LED_1, 0);
    bspInit();
    osInit();
}
