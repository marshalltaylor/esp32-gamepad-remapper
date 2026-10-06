#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "bsp.h"
#define localPrintf printf
#include "testWrappers.h"
#include "core_util.h"

void
test_construction (void)
{
    localPrintf(
        "This test proves that the data size will always be 32 bit aligned\n");
    UnitWrapper *pUnit;

    localPrintf("Create 1 byte, 5 slots\n");
    pUnit = new UnitWrapper(1, 5);
    CU_ASSERT(5 == pUnit->get_poolSize());
    CU_ASSERT(4 == pUnit->get_dataWidth());
    delete (pUnit);

    localPrintf("Create 2 byte, 5 slots\n");
    pUnit = new UnitWrapper(2, 5);
    CU_ASSERT(5 == pUnit->get_poolSize());
    CU_ASSERT(4 == pUnit->get_dataWidth());
    delete (pUnit);

    localPrintf("Create 3 byte, 5 slots\n");
    pUnit = new UnitWrapper(3, 5);
    CU_ASSERT(5 == pUnit->get_poolSize());
    CU_ASSERT(4 == pUnit->get_dataWidth());
    delete (pUnit);

    localPrintf("Create 4 byte, 5 slots\n");
    pUnit = new UnitWrapper(4, 5);
    CU_ASSERT(5 == pUnit->get_poolSize());
    CU_ASSERT(4 == pUnit->get_dataWidth());
    delete (pUnit);

    localPrintf("Create 5 byte, 5 slots\n");
    pUnit = new UnitWrapper(5, 5);
    CU_ASSERT(5 == pUnit->get_poolSize());
    CU_ASSERT(8 == pUnit->get_dataWidth());
    delete (pUnit);
}

void
test_array_indexing (void)
{
    localPrintf("This test exercises the bit indexing for slot usage\n");
    UnitWrapper *pUnit;

    localPrintf("Create u16 for general testing\n");
    pUnit = new UnitWrapper(sizeof(uint16_t), 20);
    // Override the in use table/bits
    localPrintf("Set some slots 'used' using internal methods\n");
    pUnit->use_inUseWrite(3, 1);
    pUnit->use_inUseWrite(5, 1);
    pUnit->use_inUseWrite(14, 1);
    uint8_t *bitArray = pUnit->get_inUseBits();
    // b0 of byte 0 is slot 0 -- so the hex values here are filipped
    CU_ASSERT(0x28 == bitArray[0]);
    CU_ASSERT(0x40 == bitArray[1]);
    CU_ASSERT(0x00 == bitArray[2]);
    core_print_pool_info(pUnit);
    localPrintf("'use' more slots and see that they fill in the array\n");
    uint16_t *pObj;
    pObj = (uint16_t *)pUnit->_malloc();
    CU_ASSERT(pObj != NULL);
    pObj = (uint16_t *)pUnit->_malloc();
    CU_ASSERT(pObj != NULL);
    pObj = (uint16_t *)pUnit->_malloc();
    CU_ASSERT(pObj != NULL);
    pObj = (uint16_t *)pUnit->_malloc();
    CU_ASSERT(pObj != NULL);
    pObj = (uint16_t *)pUnit->_malloc();
    CU_ASSERT(pObj != NULL);
    CU_ASSERT(0x7F == bitArray[0]);
    CU_ASSERT(0x40 == bitArray[1]);
    CU_ASSERT(0x00 == bitArray[2]);
    core_print_pool_info(pUnit);
    delete (pUnit);
}

void
test_add_remove (void)
{
    localPrintf(
        "This test writes too much data, then takes too much data out\n");
    UnitWrapper *pUnit;

    localPrintf("Create u16 for general testing\n");
    pUnit = new UnitWrapper(sizeof(uint16_t), 12);

    poolErrorStats_t *eStat = pUnit->getErrorStats();

    CU_ASSERT(0 == eStat->failedPalloc);
    CU_ASSERT(0 == eStat->failedPfree);

    uint16_t *pObj[20];
    for (int i = 0; i < 12; i++)
    {
        CU_ASSERT(i == pUnit->slotsUsed());
        pObj[i] = (uint16_t *)pUnit->_malloc();
        CU_ASSERT(pObj[i] != NULL);
    }
    localPrintf("List full, adding one more...\n");
    pObj[12] = (uint16_t *)pUnit->_malloc();
    CU_ASSERT(pObj[12] == NULL);
    CU_ASSERT(12 == pUnit->slotsUsed());
    CU_ASSERT(1 == eStat->failedPalloc);
    CU_ASSERT(0 == eStat->failedPfree);

    localPrintf("Free all, start from middle\n");
    for (int i = 6; i < 12; i++)
    {
        pUnit->_free(pObj[i]);
        CU_ASSERT(12 - i + 5 == pUnit->slotsUsed());
    }
    for (int i = 5; i >= 0; i--)
    {
        pUnit->_free(pObj[i]);
        CU_ASSERT(i == pUnit->slotsUsed());
    }

    localPrintf("Free two that have already been freed\n");
    pUnit->_free(pObj[4]);
    CU_ASSERT(0 == pUnit->slotsUsed());
    pUnit->_free(pObj[9]);
    CU_ASSERT(0 == pUnit->slotsUsed());

    localPrintf("Check error stats\n");
    CU_ASSERT(1 == eStat->failedPalloc);
    CU_ASSERT(2 == eStat->failedPfree);

    localPrintf("Clear error stats\n");
    pUnit->clearErrorStats();
    CU_ASSERT(0 == eStat->failedPalloc);
    CU_ASSERT(0 == eStat->failedPfree);

    delete (pUnit);
}

int
main ()
{
    localPrintf("CUnit test\n");
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("pool_tests", 0, 0);

    CU_add_test(suite, "test_construction", test_construction);
    CU_add_test(suite, "test_array_indexing", test_array_indexing);
    CU_add_test(suite, "test_add_remove", test_add_remove);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    uint8_t numTestsFailed = CU_get_number_of_tests_failed();

    CU_cleanup_registry();

    return numTestsFailed;
}
