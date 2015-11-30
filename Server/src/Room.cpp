#include "Room.hpp"
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include "MessageType.hpp"

using namespace JC9;

Room::Room() : clients(), game(), selector()
{

}

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
    Player* player = game.AddPlayer();

    auto cards = player->GetCards();
    sf::Packet packet;
    for (auto card : cards)
    {
        packet << MessageType::CardPicked << card;
        client->send(packet);
        packet.clear();
    }

    clients.emplace(player, client);
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
                    {
                        Card selectedCard;
                        packet >> selectedCard;
                        if (client.first == game.GetPlayingPlayer())
                        {
                            sf::Packet response;
                            if (game.CanPlayCard(selectedCard) && game.PlayCard(selectedCard))
                            {
                                response << MessageType::CardPlayed << selectedCard << game.GetTotal();
                                for (auto otherClient : clients)
                                {
                                    otherClient.second->send(response);
                                }

                                response.clear();
                                const Player* nextPlayer = game.GetPlayingPlayer();
                                if (game.CanPlay(nextPlayer))
                                {
                                    response << MessageType::YourTurn;
                                    clients[nextPlayer]->send(response);
                                }
                                else
                                {
                                    for (auto otherClient : clients)
                                    {
                                        response << MessageType::GameFinished;
                                        if (otherClient.first != nextPlayer)
                                            response << "V";
                                        else
                                            response << "D";
                                        otherClient.second->send(response);
                                    }
                                }
                            }
                            else
                            {
                                packet << MessageType::InvalidCard;
                                client.second->send(packet);
                            }
                        }
                    }
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
