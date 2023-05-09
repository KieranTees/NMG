#ifndef SERVER_H
#define SERVER_H


class Server
{
    public:
        Server();
        virtual ~Server();
        void run();
        unsigned int getPort();

    protected:

    private:
    unsigned int local_port = 0;
};

#endif // SERVER_H
