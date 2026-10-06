/* Includes ------------------------------------------------------------------*/
#include "globals.h"

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#define localPrintf bspPrintf
/* Definitions -- (Global Access) --------------------------------------------*/

View       view;
Controller controller;

Pool pool_20(20, 500);
Pool pool_32(32, 50);

// Unknown range on this timer -- see bsp.h?
uint32_t hwTimerInterval = 100;

/* References ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void
hwTimerCallback (void)
{
    ////This is where midi ticks are counted and the information is given to
    /// time digesting modules
    // int32_t dT = midiClock.getDeltaTicks();
    // if(dT > 0)
    //{
    //     player.process(dT);
    //     //arp.process(dT);
    // }
}

// Usage:
//  This function doesn't have a prototype because the header that would
//  have it (globals.h) has c++ linkage which will cause problems if included
//  in c code.
//
//  Add 'extern void globalsInit(void);' to calling file
extern "C" void
globalsInit (void)
{

    // Write our function address into the hw timer
    // timer3setPeriod(hwTimerInterval);
    // timer3TickCallback = hwTimerCallback;
}
