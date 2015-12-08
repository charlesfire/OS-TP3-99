#ifndef SERVER_HPP
#define SERVER_HPP

#include <future>
#include <thread>
#include <tuple>
#include <vector>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>

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
            sf::TcpListener listener;
            std::vector<std::tuple<std::thread, Room*>> rooms;
            sf::SocketSelector selector;
    };
}

#endif // SERVER_HPP
