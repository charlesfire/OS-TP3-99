#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <SFML/Network/TcpListener.hpp>
#include "Room.hpp"

namespace JC9
{
    class Server final
    {
        public:
            Server();
            virtual ~Server();
            void Run();
        private:
            sf::TcpListener listener;
            std::vector<Room> rooms;
    };
}

#endif // SERVER_HPP
