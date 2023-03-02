#include "Rolfor.h"
#include "Dan.h"

#include <iostream>
#include <thread>
#include <cmath>

Rolfor::Rolfor()
{
    //ctor
}

Rolfor::~Rolfor()
{
    //dtor
}

void Rolfor::RolRun(Dan& dan)
{
    srand(time(nullptr));
    float chance = round((CV / dan.GetAC()) * 100);
    while (HP > 0 && !rolWon)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 500 + 500));
        if (rand() % 100 <= chance)
        {
            std::cout << "Rolfor: HIT" << std::endl;
            dan.SetHP(CV);
            std::cout << "Dan HP: " << dan.GetHP() << std::endl;
        }
        else
            std::cout << "Rolfor: MISSED" << std::endl;
        if (dan.GetHP() <= 0)
            rolWon = true;
    }
    if (rolWon == true)
        std::cout << "Rolfor is victorious" << std::endl;
    else
        std::cout << "Rolfor has Died" << std::endl;
}
