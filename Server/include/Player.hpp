#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "Card.hpp"

namespace JC9
{
    class Player
    {
        public:
            Player() = default;
            ~Player() = default;
            void AddCard(const Card& card);
            bool HasCard(const Card& card)const;
            void RemoveCard(const Card& card);
        private:
            std::vector<Card> cards;
    };
}

#endif // PLAYER_HPP
