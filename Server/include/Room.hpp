#ifndef ROOM_HPP
#define ROOM_HPP

#include <map>
#include <utility>
#include <SFML/Network/SocketSelector.hpp>
#include "Game.hpp"
#include "sqlite3pp.h"

namespace sf
{
    class TcpSocket;
}

namespace JC9
{
    class Player;

    class Room final
    {
        public:
            Room();
            ~Room();
            void AddClient(sf::TcpSocket* client, const std::string& username, sf::Uint16 score);
            void EndGame(const Player* loser);
            bool IsPlaying()const;
            void PlayGame();
            unsigned int GetClientCount()const;
        private:
            std::map<const Player*, sf::TcpSocket*> clients;
            sqlite3pp::database db;
            Game game;
            bool isPlaying;
            sf::SocketSelector selector;
    };
}

#endif // ROOM_HPP
