#include "MessageType.hpp"

using namespace JC9;

sf::Packet& operator<<(sf::Packet& packet, const JC9::MessageType& type)
{
    sf::Uint8 temp = static_cast<sf::Uint8>(type);
    return packet << temp;
}

sf::Packet& operator>>(sf::Packet& packet, JC9::MessageType& type)
{
    sf::Uint8 temp;
    packet >> temp;
    type = static_cast<MessageType>(temp);
    return packet;
}
