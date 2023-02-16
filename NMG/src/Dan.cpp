#include "Dan.h"
#include "Rolfor.h"

#include <iostream>
#include <thread>
#include <cmath>

Dan::Dan()
{
    //ctor
}

Dan::~Dan()
{
    //dtor
}

void Dan::DanRun(Rolfor& rol)
{
    srand(time(nullptr));
    float chance = round((CV / rol.GetAC()) * 100);
    while (HP > 0 && !danWon)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 500 + 500));
        if (rand() % 100 <= chance)
        {
            std::cout << "Dan: HIT" << std::endl;
            rol.SetHP(CV);
            std::cout << "Rolfor HP: " << rol.GetHP() << std::endl;
        }
        else
            std::cout << "Dan: MISSED" << std::endl;
        if (rol.GetHP() <= 0)
            danWon = true;
    }
    if (danWon == true)
        std::cout << "Dan is victorious" << std::endl;
    else
        std::cout << "Dan has Died" << std::endl;
}
