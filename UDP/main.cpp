#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <cmath>
#include <iostream>
#include <thread>

void server()
{
    while (true)
    {
        // Create a socket and bind it to the port 4300
        sf::UdpSocket socket;
        socket.bind(4300);
        // Receive a message from anyone
        char buffer[1024];
        std::size_t received = 0;
        sf::IpAddress sender;
        unsigned short port;
        socket.receive(buffer, sizeof(buffer), received, sender, port);
        std::cout << sender.toString() << " said: " << buffer << std::endl;
        // Send an answer
        std::string message = "Welcome " + sender.toString();
        socket.send(message.c_str(), message.size() + 1, sender, port);
    }
}

void client()
{
    // Create a socket and bind it to the port 4301
    sf::UdpSocket socket;
    //socket.bind(4301);
    // Send a message to 152.105.67.170 on port 4300
    std::string message = "Hello, I am " + sf::IpAddress::getLocalAddress().toString();
    socket.send(message.c_str(), message.size() + 1, "152.105.67.255", 4300);
    // Receive an answer
    char buffer[1024];
    std::size_t received = 0;
    sf::IpAddress sender;
    unsigned short port;
    socket.receive(buffer, sizeof(buffer), received, sender, port);
    std::cout << sender.toString() << " said: " << buffer << std::endl;
}

int main()
{
    std::thread serverThread(&server);
    std::thread clientThread(&client);
    serverThread.join();
    clientThread.join();
    return 0;
}
