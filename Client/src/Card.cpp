#include "Card.hpp"
#include <limits>

using namespace JC9;

Card::Card() : number(std::numeric_limits<sf::Uint8>::max()), type(Card::Type::Count)
{

}

Card::Card(Type type, sf::Uint8 number) : number(number), type(type)
{

}

Card::Card(const Card& other) : number(other.number), type(other.type)
{

}

Card& Card::operator=(Card other)
{
    number = other.number;
    type = other.type;
    return *this;
}

bool Card::operator==(const JC9::Card& other)const
{
    return (other.GetNumber() == number && other.GetType() == type);
}

sf::Uint8 Card::GetNumber()const
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

Card::Type Card::GetType()const
{
    return type;
}

sf::Packet& operator<<(sf::Packet& packet, const JC9::Card& card)
{
    return packet << static_cast<sf::Uint8>(card.GetType()) << card.GetNumber();
}

sf::Packet& operator>>(sf::Packet& packet, JC9::Card& card)
{
    sf::Uint8 type;
    sf::Uint8 number;
    packet >> type >> number;
    card.SetType(static_cast<Card::Type>(type));
    card.SetNumber(number);
    return packet;
}
