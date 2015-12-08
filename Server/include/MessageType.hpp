#ifndef MESSAGE_TYPE_HPP
#define MESSAGE_TYPE_HPP

#include <SFML/Network/Packet.hpp>

namespace JC9
{
    enum MessageType : sf::Uint8 {Connection, ConnectionSucceeded, ConnectionFailed, CardPicked, CardSelected, CardPlayed, GameFinished, Leaderboard, InvalidCard, YourTurn, GameStarted, PlayerDisconnected};
}

sf::Packet& operator<<(sf::Packet& packet, const JC9::MessageType& type);

sf::Packet& operator>>(sf::Packet& packet, JC9::MessageType& type);

#endif // MESSAGE_TYPE_HPP
