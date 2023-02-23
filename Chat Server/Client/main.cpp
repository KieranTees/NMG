#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <cmath>
#include <iostream>
#include <thread>

void client()
{
    sf::TcpSocket socket;
    sf::IpAddress address("152.105.67.169");
    sf::Socket::Status status;
    socket.connect(address, 4300);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error Connecting" << std::endl;
        return;
    }
    sf::Packet packet;
    packet << "Message";
    status = socket.send(packet);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error Sending" << std::endl;
        return;
    }
    packet.clear();
    status = socket.receive(packet);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error Receiving" << std::endl;
        return;
    }
    std::string message;
    packet >> message;
    std::cout << message;
    socket.disconnect();
}

int main()
{
    std::thread clientThread(&client);
    clientThread.join();
    return 0;
}
