#include "queue.h"
#include "receiver.h"
#include "util.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

int main()
{
    std::shared_ptr<sf::TcpSocket> socket = std::make_shared<sf::TcpSocket>();
    // auto connect = [&] { return socket->connect(sf::IpAddress::getLocalAddress(), PORT); };
    // net_run(connect, "connect");
    sf::Socket::Status status = socket->connect(sf::IpAddress::getLocalAddress(), PORT);
    if (status != sf::Socket::Done)
    {
        return 1;
    }
    std::cout << "Connected\n";
    Queue<std::string> queue;
    // TODO launch a receiver thread to receive messages from the server.
    std::string s = "";
    while (1)
    {
        std::getline(std::cin, s);
        std::cout << "Sending: \"" << s << "\"" << std::endl;
        // TODO send messages to the server
    }
    return 0;
}
