#ifndef ROLFOR_H
#define ROLFOR_H

class Dan;

class Rolfor
{
    public:
        Rolfor();
        virtual ~Rolfor();
        void RolRun(Dan& dan);

        float GetAC() {return AC;}
        float GetHP() {return HP;}
        float GetCV() {return CV;}

        void SetHP(float dmg) {HP -= dmg;}

        int Hit();

        bool rolWon {false};
    protected:

    private:
        float HP {80};
        float AC {18};
        float CV {16};
};

#endif // ROLFOR_H
