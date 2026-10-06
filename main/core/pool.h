#ifndef POOL_H
#define POOL_H

#include <stdint.h>

#include "memoryController.h"

typedef void *pool_t;

struct poolErrorStats_t
{
    uint32_t failedPalloc;
    uint32_t failedPfree;
};

class Pool : public AllocInterface
{
public:
    Pool(int dataWidthVal, int poolSizeVal);
    virtual ~Pool(void);
    void             *_malloc(int size);
    void             *_malloc(void);
    void              _free(void *p);
    int               slotsUsed(void);
    poolErrorStats_t *getErrorStats(void);
    void              clearErrorStats(void);

    bool inUseRead(int index);
    // Should be protected
    int poolSize;
    int dataWidth;

protected:
    void             inUseWrite(int index, bool used);
    uint8_t         *inUseBits;
    uint8_t         *poolData;
    poolErrorStats_t errorStats;
};

class Debug_Pool : public Pool
{
public:
    using Pool::Pool;
};

#endif
