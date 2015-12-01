#ifndef SERVER_HPP
#define SERVER_HPP

#include <future>
#include <thread>
#include <tuple>
#include <vector>
#include <SFML/Network/TcpListener.hpp>

namespace JC9
{
    class Room;

    class Server final
    {
        public:
            Server();
            virtual ~Server();
            void Run();
        private:
            sf::TcpListener listener;
            std::vector<std::tuple<std::thread, Room*>> rooms;
    };
}

#endif // SERVER_HPP
