#include "Server.hpp"
#include <SFML/Network/TcpSocket.hpp>

using namespace JC9;

Server::Server() : listener(), playingRooms()
{
    listener.listen(6666);
}

Server::~Server()
{
    playingRooms.clear();
}

void Server::Run()
{
    while (true)
    {
        Room* room = new Room();
        while (room->GetClientCount() < 4)
        {
            sf::TcpSocket* client = new sf::TcpSocket();
        }
    }
}
