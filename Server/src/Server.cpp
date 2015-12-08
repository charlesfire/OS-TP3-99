#include "Server.hpp"
#include <iostream>
#include <chrono>
#include <SFML/Network/TcpSocket.hpp>
#include "CryptedPacket.hpp"
#include "MessageType.hpp"
#include "Room.hpp"

using namespace JC9;

Server::Server(const unsigned int port) : listener(), rooms(), selector()
{
    listener.listen(port);
    selector.add(listener);
}

Server::~Server()
{
    for (auto& room : rooms)
        delete std::get<1>(room);
    rooms.clear();
}

void Server::Run()
{
    while (true)
    {
        /*for (auto it = rooms.begin(); it != rooms.end(); it++)
        {
            if (std::get<0>(*it).wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            {
                std::cout << "Room closed" << std::endl;
                delete std::get<2>(*it);
                rooms.erase(it);
            }
        }*/

        // TODO : delete unused threads

        std::cout << "New room created" << std::endl;
        Room* room = new Room();
        while (selector.wait())
        {
            if (selector.isReady(listener))
            {
                sf::TcpSocket* connection = new sf::TcpSocket();
                if (listener.accept(*connection) != sf::Socket::Status::Done)
                {
                    std::cout << "Connection error" << std::endl;
                    connection->disconnect();
                    delete connection;
                }
                else
                {
                    connections.push_back(connection);
                }
            }
            else
                for (auto connection : connections)
                {
                    if (selector.isReady(*connection))
                    {
                        CryptedPacket packet;
                        auto status = connection->receive(packet);
                        if (status == sf::Socket::Status::Disconnected || sf::Socket::Status::Error)
                        {
                            // TODO : disconnect player
                        }
                        else
                        {
                            CryptedPacket response;
                            MessageType type;
                            packet >> type;
                            if (type == MessageType::Connection)
                            {

                            }
                            else
                                response << MessageType::ConnectionFailed;

                            connection->send(response);
                        }
                    }
                }
        }
        /*Room* room = new Room();
        while (room->GetClientCount() < 3)
        {
            sf::TcpSocket* client = new sf::TcpSocket();
            if (listener.accept(*client) != sf::Socket::Status::Done)
            {
                std::cout << "Connection error" << std::endl;
                client->disconnect();
                delete client;
            }
            else
            {
                std::cout << "Client added" << std::endl;
                room->AddClient(client);
            }
        }*/

        std::cout << "Game started" << std::endl;
        std::packaged_task<void()> task(std::bind(&Room::PlayGame, room));
        rooms.push_back(std::make_tuple(std::thread(std::move(task)), room));
    }
}
