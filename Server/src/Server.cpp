#include "Server.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <SFML/Network/TcpSocket.hpp>

#include <SFML/Network.hpp>
#include "CryptedPacket.hpp"
#include "Encryptor.hpp"
#include "MessageType.hpp"
#include "Room.hpp"

using namespace JC9;

Server::Server(const unsigned int port) : connections(), db("TP3.db"), listener(), rooms(), selector()
{
    listener.listen(port);
    selector.add(listener);
}

Server::~Server()
{
    for (auto connection : connections)
    {
        connection->disconnect();
        delete connection;
        selector.remove(*connection);
        auto it = std::find(connections.begin(), connections.end(), connection);
        if (it != connections.end())
            connections.erase(it);
    }

    for (auto& room : rooms)
        delete room.first;

    db.execute("UPDATE Players SET Connected = 0");
}

void Server::Run()
{
    std::cout << "New room created" << std::endl;
    Room* room = new Room();
    while (true)
    {
        selector.wait();
        std::vector<sf::TcpSocket*> toRemove;
        if (selector.isReady(listener))
        {
            sf::TcpSocket* connection = new sf::TcpSocket();
            if (listener.accept(*connection) != sf::Socket::Status::Done)
            {
                std::cout << "Connection error" << std::endl;
                connection->disconnect();
                toRemove.push_back(connection);
            }
            else
            {
                std::cout << "New client" << std::endl;
                selector.add(*connection);
                connections.push_back(connection);
            }
        }
        else
        {
            for (auto connection : connections)
            {
                if (selector.isReady(*connection))
                {
                    CryptedPacket packet;
                    auto status = connection->receive(packet);
                    if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error)
                    {
                        std::cout << "Client disconnected" << std::endl;
                        connection->disconnect();
                        toRemove.push_back(connection);
                    }
                    else
                    {
                        CryptedPacket response;
                        MessageType type;
                        packet >> type;
                        if (type == MessageType::Connection)
                        {
                            std::string username, password;
                            packet >> username >> password;
                            std::cout << username << " : " << password << std::endl;
                            password = Encryptor::Crypt(password);
                            std::cout << password << std::endl;
                            sqlite3pp::query qry(db, "SELECT count(*), Score FROM Players WHERE Username = ? AND Password = ? AND Connected = 0");
                            qry.bind(1, username.c_str());
                            qry.bind(2, password.c_str());
                            sf::Uint16 count, score;
                            (*qry.begin()).getter() >> count >> score;
                            if (count)
                            {
                                std::cout << "Connection succeeded" << std::endl;
                                response << MessageType::ConnectionSucceeded;
                                room->AddClient(connection, username, score);
                                sqlite3pp::command cmd(db, "UPDATE Players SET Connected = 1 WHERE Username = ?");
                                cmd.bind(1, username.c_str());
                                cmd.execute();
                                toRemove.push_back(connection);
                                if (room->GetClientCount() == 2)
                                {
                                    std::cout << "Game started" << std::endl;
                                    rooms.emplace(std::make_pair(room, std::thread(std::bind(&Room::PlayGame, room))));
                                    std::cout << "New room created" << std::endl;
                                    room = new Room();
                                }
                            }
                            else
                            {
                                std::cout << "Connection failed" << std::endl;
                                response << MessageType::ConnectionFailed;
                            }
                        }
                        else
                        {
                            std::cout << "Connection failed" << std::endl;
                            response << MessageType::ConnectionFailed;
                        }

                        connection->send(response);
                    }
                }
            }
        }

        for (auto remove : toRemove)
        {
            selector.remove(*remove);
            auto it = std::find(connections.begin(), connections.end(), remove);
            if (it != connections.end())
                connections.erase(it);
            if (remove->getRemoteAddress().toInteger() == 0)
                delete remove;
        }
        toRemove.clear();

        std::vector<Room*> endedRooms;
        for (auto& runningRoom : rooms)
        {
            if (!runningRoom.first->IsPlaying())
            {
                endedRooms.push_back(runningRoom.first);
                runningRoom.second.join();
            }
        }

        for (auto endedRoom : endedRooms)
        {
            rooms.erase(endedRoom);
            delete endedRoom;
        }
    }
}
