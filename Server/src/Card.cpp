#include "Card.hpp"
#include <limits>

using namespace JC9;

Card::Card() : number(std::numeric_limits<sf::Uint8>::max()), type(Card::Type::Spade)
{

}

Card::Card(Type type, sf::Uint8 number) : number(number), type(type)
{

}

Card::Card(const Card& other) : number(other.number), type(other.type)
{

}

sf::Uint8 Card::GetNumber() const
{
    return number;
}

void Card::SetType(Card::Type type)
{
    this->type = type;
}

void Card::SetNumber(sf::Uint8 number)
{
    this->number = number;
}

Card::Type Card::GetType() const
{
    return type;
}

inline bool operator == (const JC9::Card& left, const JC9::Card& right)
{
    return (left.GetNumber() == right.GetNumber() && left.GetType() == right.GetType());
}

sf::Packet& operator << (sf::Packet& packet, const JC9::Card& card)
{
    return packet << static_cast<sf::Uint8>(card.GetType()) << card.GetNumber();
}

sf::Packet& operator >> (sf::Packet& packet, JC9::Card& card)
{
    sf::Uint8 type;
    sf::Uint8 number;
    packet >> type >> number;
    card.SetType(static_cast<Card::Type>(type));
    card.SetNumber(number);
    return packet;
}
