/* Includes ------------------------------------------------------------------*/
#include "pool.h"

/* References ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void
core_print_pool_info (Pool *pPool)
{
    printf("Info on poolData @ %p\n", (void *)pPool);
    printf("  Bytes in type: %d\n", pPool->dataWidth);
    printf("  number of slots: %d\n", pPool->poolSize);
    printf("  slots used: %d (%.2f%%)\n",
           pPool->slotsUsed(),
           100 * (float)pPool->slotsUsed() / pPool->poolSize);
    printf("  Total bytes of data: %d\n", pPool->poolSize * pPool->dataWidth);
    printf("  Slot bits: ");
    for (int i = 0; i < pPool->poolSize; i++)
    {
        printf("%d", pPool->inUseRead(i));
    }
    printf("\n");

    // Errors
    poolErrorStats_t *pErr = pPool->getErrorStats();
    printf("Errors, palloc: %d, pfree: %d\n",
           pErr->failedPalloc,
           pErr->failedPfree);
}
