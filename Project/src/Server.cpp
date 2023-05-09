#include "Server.h"

Server::Server()
{

}
Server::~Server()
{

}
void Server::run()
{
    sf::UdpSocket socket;
    std::cout << "Please type in the server port: ";
    std::cin >> local_port;

    //Checks if th server can bind to specified port
    if (socket.bind(local_port) != sf::Socket::Done)
    {
        std::cerr << "Binding error" << std::endl;
        return;
    }
    else
    {
        std::cout << "Server created" << std::endl;
    }

    sf::IpAddress sender;
    unsigned short remote_port;
    std::vector <sf::IpAddress> playerLog;
    while(true)
    {
        sf::Packet packet;
        sf::Socket::Status status = socket.receive(packet, sender, remote_port);
        //std::cout << sender << std::endl;
        std::string s;
        packet >> s;
        //Adds player to player vector when they first connect
        if (s == "Connecting")
        {
            playerLog.push_back(sender);
            std::cout << "Player count: " << playerLog.size() << std::endl;
            //playerCount = playerLog.size();
        }
        //Assigns player id based off of their position in the player vector
        for (int i = 0; i < playerLog.size(); i++)
        {
            if (sender == playerLog[i])
            {
                playerNumber = i + 1;
                break;
            }
        }
        //Checks if received message is player position to avoid message spam
        if (s.find('x') != std::string::npos || s.find('y') != std::string::npos)
        {

        }
        else
        {
            std::cout << "Player " << playerNumber << ": " << s << std::endl;
        }
        packet.clear();
        //Sends message back to client
        packet << "bcast";
        socket.send(packet, sender, remote_port);
    }
}
unsigned int Server::getPort()
{
    return local_port;
}
