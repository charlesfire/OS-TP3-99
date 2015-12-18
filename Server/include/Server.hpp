#ifndef SERVER_HPP
#define SERVER_HPP

#include <future>
#include <unordered_map>
#include <thread>
#include <vector>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include "sqlite3pp.h"

namespace JC9
{
    class Room;

    class Server final
    {
        public:
            Server() = delete;
            Server(const unsigned int port);
            virtual ~Server();
            void Run();
        private:
            std::vector<sf::TcpSocket*> connections;
            sqlite3pp::database db;
            sf::TcpListener listener;
            std::unordered_map<Room*, std::thread> rooms;
            sf::SocketSelector selector;
    };
}

#endif // SERVER_HPP
