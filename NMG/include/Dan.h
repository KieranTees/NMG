#ifndef DAN_H
#define DAN_H

//#include "Rolfor.h"

class Rolfor;

class Dan
{
    public:
        Dan();
        virtual ~Dan();
        void DanRun(Rolfor& rol);
        void test() {}

        float GetAC() {return AC;}
        float GetHP() {return HP;}
        float GetCV() {return CV;}

        void SetHP(float dmg) {HP -= dmg;}

        int Hit();

        bool danWon {false};
    protected:

    private:
        float HP {160};
        float AC {22};
        float CV {10};
};

#endif // DAN_H
