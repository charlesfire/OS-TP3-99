#ifndef SERVER_HPP
#define SERVER_HPP

#include <thread>
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
            std::vector<Room*> rooms;
            std::vector<std::thread> playingRooms;
    };
}

#endif // SERVER_HPP
