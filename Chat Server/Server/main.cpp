#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <cmath>
#include <iostream>
#include <thread>

void server()
{
    sf::TcpListener listener;
    sf::Socket::Status status;
    status = listener.listen(4300);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error Listening" << std::endl;
        return;
    }
    sf::TcpSocket socket;
    status = listener.accept(socket);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error Accepting" << std::endl;
        return;
    }

    sf::Packet packet;
    status = socket.receive(packet);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error Receiving" << std::endl;
        return;
    }
    std::string message;
    packet >> message;
    std::cout << message << std::endl;
    packet.clear();
    packet << "Message Received";
    status = socket.send(packet);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error Sending" << std::endl;
        return;
    }

    socket.disconnect();
    listener.close();
}

int main()
{
    std::thread serverThread(&server);
    serverThread.join();
    return 0;
}
