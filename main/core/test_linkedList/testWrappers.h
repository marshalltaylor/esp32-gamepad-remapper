#ifndef TESTWRAPPERS_H
#define TESTWRAPPERS_H

/* Includes -- STD -----------------------------------------------------------*/

/* Includes -- App -----------------------------------------------------------*/
#include "exampleList.h"

class UnitWrapper : public ExampleList
{
public:
    using ExampleList::ExampleList;
    void printInfo(void);
    void printBookmarks(void);
};

#endif
