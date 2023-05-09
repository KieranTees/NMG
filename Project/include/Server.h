#ifndef SERVER_H
#define SERVER_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>


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
        int playerCount = 0;
        bool playerCheck = false;
        std::string packetBuffer;
        int playerNumber = 0;
};

#endif // SERVER_H
