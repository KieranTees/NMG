#include "accepter.h"
#include "queue.h"
#include "list.h"
#include "receiver.h"
#include "util.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

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

int client()
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
    Receiver receiver(socket, queue);
    std::thread receiverThread(&Receiver::recv_loop(), receiver);
    std::string s = "";
    while (1)
    {
        std::getline(std::cin, s);
        std::cout << "Sending: \"" << s << "\"" << std::endl;
        queue.push(s);
        break;
        // TODO send messages to the server
    }
    return 0;
}

int main()
{
    std::thread serverThread (server);
    std::thread clientThread (&client);

    serverThread.join();
    clientThread.join();
}
