#include "Server.h"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

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
    while(true)
    {
        sf::Packet packet;
        sf::Socket::Status status = socket.receive(packet, sender, remote_port);
        std::string s;
        packet >> s;
        std::cout << "Server received: " << s << std::endl;
        packet.clear();
        packet << "bcast";
        socket.send(packet, sender, remote_port);
    }
}
unsigned int Server::getPort()
{
    return local_port;
}
