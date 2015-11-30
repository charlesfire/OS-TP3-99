#ifndef ROOM_HPP
#define ROOM_HPP

#include <map>
#include <utility>
#include <SFML/Network/SocketSelector.hpp>
#include "Game.hpp"
#include "Player.hpp"

namespace sf
{
    class TcpSocket;
}

namespace JC9
{
    class Room final
    {
        public:
            Room();
            ~Room();
            void AddClient(sf::TcpSocket* client);
            bool IsPlaying()const;
            void PlayGame();
            unsigned int GetClientCount()const;
        private:
            std::map<const Player*, sf::TcpSocket*> clients;
            Game game;
            sf::SocketSelector selector;
    };
}

#endif // ROOM_HPP
