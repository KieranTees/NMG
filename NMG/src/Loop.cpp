#include "Loop.h"
#include <iostream>

Loop::Loop()
{
    //ctor
}

Loop::~Loop()
{
    //dtor
}


void Loop::run()
{
    while(!hasToStop)
    {

    }
    std::cout << "Thread finished\n";
}
