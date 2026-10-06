#ifndef TESTWRAPPERS_H
#define TESTWRAPPERS_H

/* Includes -- STD -----------------------------------------------------------*/

/* Includes -- App -----------------------------------------------------------*/
#include "pool.h"

class UnitWrapper : public Debug_Pool
{
public:
    using Debug_Pool::Debug_Pool;
    int      get_poolSize(void);
    int      get_dataWidth(void);
    uint8_t *get_inUseBits(void);
    void     use_inUseWrite(int index, bool used);
};

#endif
