#include "commands.h"
#include "globals.h"
// #include "sequence_default_patterns.h"

// Connect directly to bsp.
#include "bsp.h"
#define localPrintf bspPrintf

int appProgram(int argc, char *argv[]);

commandInfo_t appCmd
    = { "app",
        "do app related things",
        "  load -- fill seq with test data (probably broken)\n  mem - pool "
        "info\n  model - data\n  seq - sequences",
        &appProgram };

static void
printPoolInfo (Pool *pPool)
{
    localPrintf("Info on poolData @ %p\n", (void *)pPool);
    localPrintf("  Bytes in type: %d\n", pPool->dataWidth);
    localPrintf("  number of slots: %d\n", pPool->poolSize);
    localPrintf("  slots used: %d (%.2f%%)\n",
                pPool->slotsUsed(),
                100 * (float)pPool->slotsUsed() / pPool->poolSize);
    localPrintf("  Total bytes of data: %d\n",
                pPool->poolSize * pPool->dataWidth);
    localPrintf("  Slot bits: \n   ");
    for (int i = 0; i < pPool->poolSize; i++)
    {
        localPrintf("%d", pPool->inUseRead(i));
        if ((i + 1) % 40 == 0)
        {
            localPrintf("\n   ");
        }
        else if ((i + 1) % 10 == 0)
        {
            localPrintf(" ");
        }
    }
    localPrintf("\n");

    // Errors
    poolErrorStats_t *pErr = pPool->getErrorStats();
    localPrintf("  Errors, palloc: %d, pfree: %d\n",
                pErr->failedPalloc,
                pErr->failedPfree);
}

int
appProgram (int argc, char *argv[])
{
    bool all = false;
    if (argc == 0)
    {
        all = true;
    }

    if (all || 0 == strcmp((const char *)argv[0], "mem"))
    {
        // Print mem info
        printPoolInfo(&pool_20);
        printPoolInfo(&pool_32);
        //        mainDeck.printInfo();
    }
    if (all || 0 == strcmp((const char *)argv[0], "model"))
    {
        // Print mem info
        view.printModel();
    }
    return 0;
}
