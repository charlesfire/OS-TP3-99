#include "Server.hpp"
#include <chrono>
#include <SFML/Network/TcpSocket.hpp>
#include "Room.hpp"

using namespace JC9;

Server::Server() : listener(), rooms()
{
    listener.listen(6666);
}

Server::~Server()
{
    for (auto& room : rooms)
        delete std::get<2>(room);
    rooms.clear();
}

void Server::Run()
{
    while (true)
    {
        for (auto it = rooms.begin(); it != rooms.end(); it++)
        {
            if (std::get<0>(*it).wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            {
                delete std::get<2>(*it);
                rooms.erase(it);
            }
        }

        Room* room = new Room();
        while (room->GetClientCount() < 4)
        {
            sf::TcpSocket* client = new sf::TcpSocket();
            if (listener.accept(*client) != sf::Socket::Status::Done)
            {
                client->disconnect();
                delete client;
            }
            else
            {
                room->AddClient(client);
            }
        }

        std::packaged_task<void()> task(std::bind(Room::PlayGame, room));
        rooms.push_back(std::make_tuple(task.get_future(), std::thread(std::move(task)), room));
    }
}
