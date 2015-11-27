#ifndef ROOM_HPP
#define ROOM_HPP

#include <vector>
#include <SFML/Network/SocketSelector.hpp>

namespace sf
{
    class TcpSocket;
}

namespace JC9
{
    class Room final
    {
        public:
            Room() = default;
            virtual ~Room();
            void AddClient(sf::TcpSocket* client);
            bool IsPlaying()const;
            void PlayGame();
            unsigned int GetClientCount()const;
        private:
            sf::SocketSelector selector;
            std::vector<sf::TcpSocket*> clients;
    };
}

#endif // ROOM_HPP
