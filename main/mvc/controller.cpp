#include <stdint.h>
#include <math.h>

#include "bsp.h"

#include "model.h"
#include "view.h"
#include "controller.h"
#include "globals.h"
#include "customHardwareInterfaces.h"

// #define USE_LOGGING
#ifdef USE_LOGGING
#include "logging.h"
// Create logging object and macro for local printf
extern Logging mvcDebug;
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

Controller::Controller (void)
{
    // Build panel objects
    button_console.setHardware(new ArduinoDigitalIn(BUTTON_0), 1);
    add(&button_console);

    led_console.setHardware(new ArduinoDigitalOut(LED_1), 0);
    add(&led_console);

    // Set default states
    led_console.setState(LEDOFF);

    //    //Setup SPI
    //    controlPanelSpi.bspPin = SPI_CS;
    //    controlPanelSpi.txData = spiTxBuf;
    //    controlPanelSpi.rxData = spiRxBuf;
    //    controlPanelSpi.len = 2;
    //    controlPanelSpi.pendingXfer = false;
    //
    //    // Init data
    //    controlPanelSpi.rxData[0] = 0xFF;
    //    controlPanelSpi.rxData[1] = 0xFF;
    //
    initialized = false;
    //
    //    isThisActionARest = false;
    //
    //    // Set default model data
    //    //   (keep this last in constructor)
}

void
Controller::tick (int16_t deltaMs)
{
    if (!initialized)
    {
        //        reloadSettings();
        initialized = true;
    }

    freshenComponents(deltaMs);

    if (button_console.serviceRisingEdge())
    {
        localPrintf("play btn press\n");
        led_console.setState(LEDON);
        model.pressed = true;
    }
    if (button_console.serviceFallingEdge())
    {
        localPrintf("play btn release\n");
        led_console.setState(LEDOFF);
        model.pressed = false;
    }
}
