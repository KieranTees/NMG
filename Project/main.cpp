#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <cmath>
#include <iostream>
#include <thread>

#include "Server.h"

using namespace sf;

const int num=8; //checkpoints
// TODO: use checkpoint to make sure we are on the track.
// Slow speed when not on the track.
int points[num][2] = {300, 610,
    1270,430,
    1380,2380,
    1900,2460,
    1970,1700,
    2550,1680,
    2560,3150,
    500, 3300};

struct Car
{
    float x,y,speed,angle; int n;
    Car() {speed=2; angle=0; n=0;}
    void move()
    {
        x += sin(angle) * speed;
        y -= cos(angle) * speed;
    }
    void findTarget()
    {
        float tx=points[n][0];
        float ty=points[n][1];
        float beta = angle-atan2(tx-x,-ty+y);
        if (sin(beta)<0) angle+=0.005*speed; else angle-=0.005*speed;
        // Check if passed a checkpoint
        if ((x-tx)*(x-tx)+(y-ty)*(y-ty)<25*25) n=(n+1)%num; // TODO: simplify
    }
};

sf::IpAddress serverIP;
unsigned short serverPort;

int main()
{
    // ****************************************
    // Initialise
    srand(time(NULL));
    unsigned short remotePort;
    std::cout << "Please type in the remote port: ";
    std::cin >> remotePort;
    sf::UdpSocket udpSOCKET;

    // UDP bcast ...
    //Send broadcast
    sf::Packet packet;
    packet << "Broadcast Message";
    sf::Socket::Status status = udpSOCKET.send(packet, sf::IpAddress::Broadcast, remotePort);

    //Check status
    udpSOCKET.setBlocking(false);
    sf::Clock clock;
    while (clock.getElapsedTime().asMilliseconds() < 1000);

    //Receive
    sf::IpAddress sender;
    unsigned short port;
    packet.clear();
    status = udpSOCKET.receive(packet, sender, port);

    //If we don't get a response, make a server
    if (status != sf::Socket::Done)
    {
        std::cout << "Creating Server" << std::endl;
        Server server;
        std::thread serverThread(&Server::run, &server);
        serverThread.detach();
        while (server.getPort() == 0);
    }
    // If we did get the message from the server
    else
    {
        std::string s;
        packet >> s;
        std::cout << s << std::endl;
        std::cout << "The address of the server is: " << sender << std::endl;
    }
    serverIP = sender;
    serverPort = remotePort;

    // start recv thread, queue
    // read from the queue to note which player we are
    RenderWindow app(VideoMode(640, 480), "Car Racing Game!");
    app.setFramerateLimit(60);
    Texture t1,t2;
    t1.loadFromFile("images/background.png");
    t2.loadFromFile("images/car.png");
    t1.setSmooth(true);
    t2.setSmooth(true);
    Sprite sBackground(t1), sCar(t2);
    sBackground.scale(2,2);
    sCar.setOrigin(22, 22);
    float R=22;
    const int N=1;
    Car car[N];
    Color colors[5] = {Color::Red, Color::Green, Color::Magenta, Color::Blue, Color::White};

    // Starting positions
    for(int i=0;i<N;i++)
    {
        car[i].x=300+i*50;
        car[i].y=1700+i*80;
        car[i].speed=7+((float)i/5);
    }
    float speed=0,angle=0;
    float maxSpeed=12.0;
    float acc=0.2, dec=0.3;
    float turnSpeed=0.08;
    int offsetX=0,offsetY=0;

    // ****************************************
    // Loop

    //Initial checkpoint iterator
    int checkpointIterator = 0;

    //Checkpoints initialisation
    std::vector<RectangleShape> checkpoints;

    RectangleShape checkpointOne (Vector2f(240.0f, 20.0f));
    checkpoints.push_back(checkpointOne);
    RectangleShape checkpointTwo (Vector2f(240.0f, 20.0f));
    checkpoints.push_back(checkpointTwo);
    RectangleShape checkpointThree (Vector2f(20.0f, 240.0f));
    checkpoints.push_back(checkpointThree);
    RectangleShape checkpointFour (Vector2f(240.0f, 20.0f));
    checkpoints.push_back(checkpointFour);


    for (int i = 0; i < 4; i++)
    {
        checkpoints[i].setFillColor(sf::Color::Blue);
    }

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }
        // Step 1: user input
        bool Up=0,Right=0,Down=0,Left=0, Exit=0;
        if (Keyboard::isKeyPressed(Keyboard::Up))    Up=1;
        if (Keyboard::isKeyPressed(Keyboard::Right)) Right=1;
        if (Keyboard::isKeyPressed(Keyboard::Down))  Down=1;
        if (Keyboard::isKeyPressed(Keyboard::Left))  Left=1;
        if (Keyboard::isKeyPressed(Keyboard::Escape)) Exit = 1;
        //
        // Step 2: update
        //exits window
        if (Exit)
            app.close();
        //car movement
        if (Up && speed<maxSpeed)
        {
            if (speed < 0)  speed += dec;
            else  speed += acc;
        }
        if (Down && speed>-maxSpeed)
        {
            if (speed > 0) speed -= dec;
            else  speed -= acc;
        }
        if (!Up && !Down)
        {
            if (speed - dec > 0) speed -= dec;
            else if (speed + dec < 0) speed += dec;
            else speed = 0;
        }

        if (Right && speed!=0)  angle += turnSpeed * speed/maxSpeed;
        if (Left && speed!=0)   angle -= turnSpeed * speed/maxSpeed;
        car[0].speed = speed;
        car[0].angle = angle;
        for(int i=0;i<N;i++) car[i].move(); // We want to move the local player
        // Use socket to send a message
        // Dequeue messages
        // If player then handle ai
        for(int i=1;i<N;i++) car[i].findTarget();
        //collision
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                if (i == j)
                {
                    break;
                }
                int dx=0, dy=0;
                while (dx*dx+dy*dy<4*R*R)
                {
                    car[i].x+=dx/10.0;
                    car[i].x+=dy/10.0;
                    car[j].x-=dx/10.0;
                    car[j].y-=dy/10.0;
                    dx = car[i].x-car[j].x;
                    dy = car[i].y-car[j].y;
                    if (!dx && !dy) break;
                }
            }
        }
        // Step 3: Render
        app.clear(Color::White);

        // TODO: Stay within the limit of the map.
        //Don't show white screen.
        if (car[0].x>320 && car[0].x<(sBackground.getTextureRect().width * 2) - 320) offsetX = car[0].x-320;
        if (car[0].y>240 && car[0].y<(sBackground.getTextureRect().height * 2) - 240) offsetY = car[0].y-240;
        //Checkpoints positions
        sBackground.setPosition(-offsetX, -offsetY);
        checkpoints[0].setPosition(1240.0f- offsetX, 540.0f - offsetY);
        checkpoints[1].setPosition(1780.0f - offsetX, 1740.0f - offsetY);
        checkpoints[2].setPosition(1800.0f - offsetX, 3180.0f - offsetY);
        checkpoints[3].setPosition(200.0f - offsetX, 1780.0f - offsetY);
        app.draw(sBackground);
        for(int i=0;i<N;i++)
        {
            sCar.setPosition(car[i].x-offsetX,car[i].y-offsetY);
            sCar.setRotation(car[i].angle*180/3.141593);
            sCar.setColor(colors[i]);
            app.draw(sCar);
        }
        if (checkpointIterator == 0)
        {
            app.draw(checkpoints[0]);
            if (car[0].x - offsetX >= 1000.0f - offsetX && car[0].x - offsetX <= 1480.0f - offsetX
            && car[0].y - offsetY >= 520.0f - offsetY && car[0].y - offsetY <= 560.0f - offsetY)
            {
                checkpointIterator = 1;
                sf::Packet packet;
                packet << "Checkpoint 1/4";
                sf::Socket::Status status = udpSOCKET.send(packet, serverIP, serverPort);
                std::cout << "Checkpoint 1/4" << std::endl;
            }
        }
        else if (checkpointIterator == 1)
        {
            app.draw(checkpoints[1]);
            if (car[0].x - offsetX >= 1540.0f - offsetX && car[0].x - offsetX <= 2020.0f - offsetX
            && car[0].y - offsetY >= 1720.0f - offsetY && car[0].y - offsetY <= 1760.0f - offsetY)
            {
                checkpointIterator = 2;
                std::cout << "Checkpoint 2/4" << std::endl;
            }
        }
        else if (checkpointIterator == 2)
        {
            app.draw(checkpoints[2]);
            if (car[0].x - offsetX >= 1780.0f - offsetX && car[0].x - offsetX <= 1820.0f - offsetX
            && car[0].y - offsetY >= 2940.0f - offsetY && car[0].y - offsetY <= 3420.0f - offsetY)
            {
                checkpointIterator = 3;
                std::cout << "Checkpoint 3/4" << std::endl;
            }
        }
        else if (checkpointIterator == 3)
        {
            app.draw(checkpoints[3]);
            if (car[0].x - offsetX >= -40.0f - offsetX && car[0].x - offsetX <= 440.0f - offsetX
            && car[0].y - offsetY >= 1760.0f - offsetY && car[0].y - offsetY <= 1800.0f - offsetY)
            {
                checkpointIterator = 0;
                std::cout << "Lap Completed" << std::endl;
            }
        }
        // Victory condition
        app.display();
    }

    return 0;
}
