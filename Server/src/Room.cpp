#include "Room.hpp"
#include <iostream>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include "CryptedPacket.hpp"
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
    CryptedPacket packet;
    for (auto card : cards)
    {
        std::cout << "Card sended : " << (int)card.GetType() << (int)card.GetNumber() << std::endl;
        packet << MessageType::CardPicked << card;
        client->send(packet);
        packet.clear();
    }

    selector.add(*client);
    clients.emplace(player, client);
}

bool Room::IsPlaying()const
{
    return true;
}

void Room::PlayGame()
{
    {
        CryptedPacket pack;
        pack << MessageType::YourTurn;
        clients[game.GetPlayingPlayer()]->send(pack);
    }

    while (game.GetTotal() < 100)
    {
        selector.wait();
        for (auto client : clients)
        {
            if (selector.isReady(*client.second))
            {
                CryptedPacket packet;
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
                        std::cout << "A card has been selected" << std::endl;
                        Card selectedCard;
                        packet >> selectedCard;
                        CryptedPacket response;
                        if (client.first == game.GetPlayingPlayer())
                        {
                            std::cout << "It's the good player" << std::endl;
                            if (client.first->HasCard(selectedCard) && game.CanPlayCard(selectedCard))
                            {
                                game.PlayCard(selectedCard);
                                Card pickedCard = game.PickCard(client.first);
                                response << MessageType::CardPicked << pickedCard;
                                client.second->send(response);

                                std::cout << "Card played : " << (int)selectedCard.GetNumber() << (int)selectedCard.GetType() << std::endl;
                                response.clear();
                                response << MessageType::CardPlayed << selectedCard << game.GetTotal();
                                for (auto otherClient : clients)
                                {
                                    otherClient.second->send(response);
                                }

                                response.clear();
                                const Player* nextPlayer = game.GetPlayingPlayer();
                                if (game.CanPlay(nextPlayer))
                                {
                                    std::cout << "Next player" << std::endl;
                                    response << MessageType::YourTurn;
                                    clients[nextPlayer]->send(response);
                                }
                                else
                                {
                                    for (auto otherClient : clients)
                                    {
                                        std::cout << "Game finished" << std::endl;
                                        response << MessageType::GameFinished;
                                        if (otherClient.first != nextPlayer)
                                            response << "V";
                                        else
                                            response << "D";
                                        otherClient.second->send(response);
                                    }

                                    // TODO : Send leaderboard
                                    return;
                                }
                            }
                            else
                            {

                                response << MessageType::InvalidCard;
                                client.second->send(response);
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
