#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "bsp.h"
#define localPrintf printf
#include "testWrappers.h"

UnitWrapper *pUnit;

void
test_construction (void)
{
    localPrintf("Build a global linked list for use in this test\n");
    pUnit = new UnitWrapper(20, sizeof(exampleListItem_t));
    CU_ASSERT(NULL != pUnit);
    pUnit->clear();
}

void
test_bookmarks (void)
{
    CU_ASSERT(NULL != pUnit);
    CU_ASSERT(0 == pUnit->listLength());
    localPrintf("write 75%% space of list\n");
    exampleListItem_t  item;
    exampleListItem_t *pItem;
    for (int i = 0; i < 15; i++)
    {
        sprintf(item.name, "i*2 = %d", i * 2);
        item.number = i;
        pUnit->push(&item);
    }
    CU_ASSERT(15 == pUnit->listLength());

    localPrintf("Get bookmarks without any\n");
    CU_ASSERT(0 == pUnit->getBookmark(0));
    CU_ASSERT(0 == pUnit->getBookmark(1));

    localPrintf("Get items at bookmarks\n");
    pUnit->setBookmark(0, 6);
    pUnit->setBookmark(1, 3);

    pItem = (exampleListItem_t *)pUnit->readBookmarked(0);
    CU_ASSERT(6 == pItem->number);
    pItem = (exampleListItem_t *)pUnit->readBookmarked(1);
    CU_ASSERT(3 == pItem->number);

    localPrintf("  insert an early item\n");
    item.number = 16;
    pUnit->insert(&item, 2);
    localPrintf("  insert a mid item\n");
    item.number = 17;
    pUnit->insert(&item, 6);
    localPrintf("  insert a late item\n");
    item.number = 18;
    pUnit->insert(&item, 11);
    CU_ASSERT(18 == pUnit->listLength());

    localPrintf("Check bookmarked items didn't change\n");
    pItem = (exampleListItem_t *)pUnit->readBookmarked(0);
    CU_ASSERT(6 == pItem->number);
    pItem = (exampleListItem_t *)pUnit->readBookmarked(1);
    CU_ASSERT(3 == pItem->number);

    localPrintf("Start dropping all list items, checking for validity\n");
    localPrintf("Drop last 3\n");
    pItem = (exampleListItem_t *)pUnit->readObject(15);
    CU_ASSERT(12 == pItem->number);
    pUnit->drop(15);
    pItem = (exampleListItem_t *)pUnit->readObject(15);
    CU_ASSERT(13 == pItem->number);
    pUnit->drop(15);
    pItem = (exampleListItem_t *)pUnit->readObject(15);
    CU_ASSERT(14 == pItem->number);
    pUnit->drop(15);

    // pUnit->printList();

    localPrintf("Drop bookmarked\n");
    CU_ASSERT(8 == pUnit->getBookmark(0)); // 6 moved to position 8
    pUnit->drop(pUnit->getBookmark(0));
    pItem = (exampleListItem_t *)pUnit->readBookmarked(0);
    CU_ASSERT(7 == pItem->number); // position 9 (now 8) should be 7
    pUnit->drop(pUnit->getBookmark(1));
    pItem = (exampleListItem_t *)pUnit->readBookmarked(1);
    CU_ASSERT(4 == pItem->number);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(0 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(1 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(16 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(2 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(4 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(17 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(5 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(7 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(8 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(18 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(9 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(10 == pItem->number);
    pUnit->drop(0);

    pItem = (exampleListItem_t *)pUnit->readObject(0);
    CU_ASSERT(11 == pItem->number);
    pUnit->drop(0);

    CU_ASSERT(0 == pUnit->listLength());
}

void
test_length_edges (void)
{
    CU_ASSERT(NULL != pUnit);
    CU_ASSERT(0 == pUnit->listLength());
    localPrintf("Overfill it, write 23\n");
    exampleListItem_t  item;
    exampleListItem_t *pItem = NULL; //(exampleListItem_t *)0x1234;
    for (int i = 0; i < 23; i++)
    {
        item.number = 100 + i;
        pUnit->push(&item);
    }

    localPrintf("Do 1 bad read, 2 bad writes\n");
    pUnit->insert(&item, 25);
    pUnit->drop(25);
    pItem = (exampleListItem_t *)pUnit->readObject(25);
    CU_ASSERT(NULL == pItem);

    listErrorStats_t *eStat = pUnit->getErrorStats();
    CU_ASSERT(3 == eStat->listFull);
    CU_ASSERT(1 == eStat->badReadIndex);
    CU_ASSERT(2 == eStat->badWriteIndex);

    printf("Clear all data\n");
    pUnit->clear();
    pUnit->clearErrorStats();

    CU_ASSERT(0 == eStat->listFull);
    CU_ASSERT(0 == eStat->badReadIndex);
    CU_ASSERT(0 == eStat->badWriteIndex);
}

int
main ()
{
    localPrintf("CUnit test\n");
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("pool_tests", 0, 0);

    CU_add_test(suite, "test_construction", test_construction);
    CU_add_test(suite, "test_bookmarks", test_bookmarks);
    CU_add_test(suite, "test_length_edges", test_length_edges);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    uint8_t numTestsFailed = CU_get_number_of_tests_failed();

    CU_cleanup_registry();

    if (pUnit != NULL)
    {
        delete (pUnit);
    }

    return numTestsFailed;
}
