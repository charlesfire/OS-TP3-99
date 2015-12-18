#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Network/Packet.hpp>

namespace JC9
{
    class Card
    {
        public:
            enum Type : sf::Uint8 { Diamond, Heart, Club, Spade, Count};

            Card();
            Card(Type type, sf::Uint8 number);
            Card(const Card& other);
            ~Card() = default;
            Card& operator=(Card other);
            bool operator==(const JC9::Card& other)const;
            sf::Uint8 GetNumber()const;
            Type GetType()const;
            void SetNumber(sf::Uint8 number);
            void SetType(Type type);
        private:
            sf::Uint8 number;
            Type type;
    };
}

sf::Packet& operator<<(sf::Packet& packet, const JC9::Card& card);

sf::Packet& operator>>(sf::Packet& packet, JC9::Card& card);

#endif // CARD_HPP
