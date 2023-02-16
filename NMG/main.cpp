#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <cmath>
#include <iostream>
#include <thread>

#include "Loop.h"
#include "Dan.h"
#include "Rolfor.h"

using namespace sf;

void print()
{
    for (int i = 0; i < 500; i++)
    {
        std::cout << "Hi from print: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
}

int fight()
{
    Dan dan;
    Rolfor rol;

    rol.RolRun(dan);
    //dan.DanRun(rol);

    return 0;
}

int main()
{
//    std::thread t([]{print();});
//    t.detach();
//    for (int i = 0; i < 500; i++)
//    {
//        std::cout << "Hi from main: " << i << "\n";
//        std::this_thread::sleep_for(std::chrono::microseconds(50));
//    }
//    t.join();

//    srand(time(nullptr));
//
//    Loop loop;
//
//    std::thread lT0(&Loop::run, &loop);
//
//    std::thread lT1([&loop] {
//        std::this_thread::sleep_for(std::chrono::seconds(rand() % 5));
//        loop.setStop();
//    });
//
//
//    lT0.join();
//    lT1.join();

    Dan dan;
    Rolfor rol;

    //std::thread rT(&Rolfor::RolRun, &rol);
    //std::thread dT{&Dan::DanRun, &dan, rol};
    std::thread dT([&dan, &rol] {dan.DanRun(rol);});
    std::thread rT([&rol, &dan] {rol.RolRun(dan);});

    rT.join();
    dT.join();

    return 0;
}
