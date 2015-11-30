#include "Room.hpp"
#include <SFML/Network/TcpSocket.hpp>
#include "MessageType.hpp"

using namespace JC9;

Room::~Room()
{
    for (auto client : clients)
    {
        client.second->disconnect();
        delete client.second;
    }

    clients.clear();
}

void Room::AddClient(sf::TcpSocket* client)
{
    clients.emplace(game.AddPlayer(), client);
}

bool Room::IsPlaying()const
{
    return true;
}

void Room::PlayGame()
{
    while (game.GetTotal() < 100)
    {
        selector.wait();
        for (auto client : clients)
        {
            if (selector.isReady(*client.second))
            {
                sf::Packet packet;
                auto status = client.second->receive(packet);

                if (status != sf::Socket::Status::Done)
                {
                    // TODO : disconnect player;
                }

                MessageType type;
                packet >> type;

                switch (type)
                {
                    case MessageType::CardSelected:

                        break;
                    default:
                        break;
                }
            }
        }
    }
}

unsigned int Room::GetClientCount()const
{
    return clients.size();
}
