#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Network/Packet.hpp>

namespace JC9
{
    class Card final
    {
        public:
            enum Type : sf::Uint8 { Diamond, Heart, Club, Spade};

            Card();
            Card(Type type, sf::Uint8 number);
            Card(const Card& other);
            ~Card() = default;
            sf::Uint8 GetNumber()const;
            Type GetType()const;
            void SetNumber(sf::Uint8 number);
            void SetType(Type type);
        private:
            sf::Uint8 number;
            Type type;
    };
}

inline bool operator == (const JC9::Card& left, const JC9::Card& right);

sf::Packet& operator << (sf::Packet& packet, const JC9::Card& card);

sf::Packet& operator >> (sf::Packet& packet, JC9::Card& card);

#endif // CARD_HPP
