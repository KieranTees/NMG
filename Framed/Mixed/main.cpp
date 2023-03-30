#include "accepter.h"
#include "queue.h"
#include "list.h"
#include "receiver.h"
#include "util.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <cstring>

int server()
{
    Queue<std::string> queue;
    List<std::shared_ptr<sf::TcpSocket>> sockets;
    Accepter accepter(queue, sockets);
    // TODO launch an accepter thread.
    std::thread accepterThread (accepter);
    while(1)
    {
        std::string s = queue.pop();
        std::cout << "Main read: \"" << s << "\"\n";
        // TODO send to all in sockets. Be careful to synchronise.
    }
    return 0;
}

int UDPserver()
{
    sf::UdpSocket socket;
    unsigned int local_port;
    std::cout << "Please type in the server port: ";
    std::cin >> local_port;

    if (socket.bind(local_port) != sf::Socket::Done)
    {
        std::cerr << "Binding error" << std::endl;
        return 0;
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

int client()
{
    std::shared_ptr<sf::TcpSocket> socket = std::make_shared<sf::TcpSocket>();
    // auto connect = [&] { return socket->connect(sf::IpAddress::getLocalAddress(), PORT); };
    // net_run(connect, "connect");
    sf::Socket::Status status = socket->connect(sf::IpAddress("152.105.67.168"), PORT);
    if (status != sf::Socket::Done)
    {
        return 1;
    }
    std::cout << "Connected\n";
    Queue<std::string> queue;
    // TODO launch a receiver thread to receive messages from the server.
    Receiver receiver(socket, queue);
    std::thread receiverThread(&Receiver::recv_loop, receiver);
    std::string s = "";
    while (1)
    {
        std::getline(std::cin, s);
        std::cout << "Sending: \"" << s << "\"" << std::endl;
        socket->send(s.c_str(), s.size());
//        queue.push(s);
//        break;
        // TODO send messages to the server
    }

    receiverThread.join();

    return 0;
}

int UDPclient()
{
    unsigned short remote_port;
    std::cout << "Please type in the remote port: ";
    std::cin >> remote_port;
    sf::UdpSocket socket;

    // Send
    sf::Packet packet;
    packet << "broadcast message";
    sf::Socket::Status status = socket.send(packet, sf::IpAddress::Broadcast, remote_port);
    // TODO: check status

    socket.setBlocking(false);
    sf::Clock clock;
    while (clock.getElapsedTime().asMilliseconds() < 1000);

    // Receive
    sf::IpAddress sender;
    unsigned short port;
    packet.clear();
    status = socket.receive(packet, sender, port);

    // If we don't get the message from the server
    if (status != sf::Socket::Done)
    {
        std::cout << "Creating Server" << std::endl;
        std::thread(UDPserver).join();
    }

    // If we did get the message from the server
    std::string s;
    packet >> s;
    std::cout << s << std::endl;
    std::cout << "The address of the server is: " << sender << std::endl;

    while (1)
    {
        std::getline(std::cin, s);
        std::cout << "Sending: \"" << s << "\"" << std::endl;
        packet >> s;
        socket.send(packet, sender, port);
//        queue.push(s);
//        break;
        // TODO send messages to the server
    }
}

int main()
{
    // prompt the user to say server or client
    std::string choice = "";
    while (!(choice == "CLIENT" || choice == "SERVER"))
    {
        std::cout << "Client or server?" <<std::endl;
        std::cin >> choice;
        std::transform(choice.begin(), choice.end(), choice.begin(), ::toupper);
    }
    if (choice == "CLIENT")
    {
        std::thread clientThread (&UDPclient);
        clientThread.join();
    }
    else if (choice == "SERVER")
    {
        std::thread serverThread (UDPserver);

        serverThread.join();
    }

    //152.105.67.169
}
