#include "Room.hpp"

using namespace JC9;

Room::~Room()
{
    for (auto client : clients)
    {
        client->disconnect();
        delete client;
    }

    clients.clear();
}

void Room::AddClient(sf::TcpSocket* client)
{
    clients.push_back(client);
}

bool Room::IsPlaying()const
{
    return true;
}

void Room::PlayGame()
{

}

unsigned int Room::GetClientCount()const
{
    return clients.size();
}
