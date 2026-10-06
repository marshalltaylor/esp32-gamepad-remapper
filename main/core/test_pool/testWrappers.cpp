/* Includes -- App -----------------------------------------------------------*/
#include <stdio.h>

/* Includes -- Test ----------------------------------------------------------*/
#include "testWrappers.h"

/* References ----------------------------------------------------------------*/
#define SAFETY_LIMIT 10000 // Max index in case of unterminated loops

char debugStr[] = "pool";

/* Functions -----------------------------------------------------------------*/
int
UnitWrapper::get_poolSize (void)
{
    return poolSize;
}

int
UnitWrapper::get_dataWidth (void)
{
    return dataWidth;
}

uint8_t *
UnitWrapper::get_inUseBits (void)
{
    return inUseBits;
}

void
UnitWrapper::use_inUseWrite (int index, bool used)
{
    inUseWrite(index, used);
}