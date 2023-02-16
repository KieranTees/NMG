#ifndef LOOP_H
#define LOOP_H


class Loop
{
    public:
        Loop();
        virtual ~Loop();
        void run();
        void setStop() { hasToStop = !hasToStop; }

    protected:

    private:
            bool hasToStop = false;
};

#endif // LOOP_H
