#include "Room.hpp"
#include <iostream>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include "CryptedPacket.hpp"
#include "MessageType.hpp"
#include "Player.hpp"

using namespace JC9;

Room::Room() : clients(), db("TP3.db"), game(), isPlaying(true), selector()
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

void Room::AddClient(sf::TcpSocket* client, const std::string& username, sf::Uint16 score)
{
    Player* player = game.AddPlayer(username, score);

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

void Room::EndGame(const Player* loser)
{
    for (auto client : clients)
    {
        std::cout << "Game finished" << std::endl;
        CryptedPacket response;
        response << MessageType::GameFinished;
        if (client.first != loser)
            response << "V";
        else
            response << "D";
        client.second->send(response);
        response.clear();
        sqlite3pp::command cmd(db, "UPDATE Players SET Score = ? WHERE Username = ?");
        cmd.bind(1, client.first->GetScore() + ((client.first != loser)? 1 : 0));
        cmd.bind(2, client.first->GetUsername().c_str());
    }

    CryptedPacket leaderboard;
    sqlite3pp::query qry(db, "SELECT Username, Score FROM Players ORDER BY Score DESC LIMIT 5");
    for (auto entry : qry)
    {
        sf::Uint16 score;
        std::string username;
        entry.getter() >> username >> score;
        leaderboard << username << score;
    }

    for (auto client : clients)
        client.second->send(leaderboard);
    isPlaying = false;
}

bool Room::IsPlaying()const
{
    return isPlaying;
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

                if (status == sf::Socket::Status::Disconnected || sf::Socket::Status::Error)
                {
                    EndGame(client.first);
                    return;
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
                                for (auto otherClient : clients)
                                {
                                    response.clear();
                                    response << MessageType::CardPlayed << selectedCard << game.GetTotal();
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
                                    EndGame(nextPlayer);
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
