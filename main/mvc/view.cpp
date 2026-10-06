#include <stdint.h>
#include <stdio.h>

#include "globals.h"

#include "model.h"
#include "view.h"
#include "controller.h"

#define localPrintf bspPrintf

View::View (void)
{
}

void
View::tick (void)
{
}

void
View::printModel (void)
{
    localPrintf("Info on Model @ %p\n", (void *)&model);
    localPrintf("  pressed: %d\n", model.pressed);
}
