#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include "Card.hpp"

namespace JC9
{
    class Player
    {
        public:
            Player() = delete;
            Player(const std::string& username);
            ~Player() = default;
            void AddCard(const Card& card);
            const std::vector<Card>& GetCards()const;
            const std::string& GetUsername()const;
            bool HasCard(const Card& card)const;
            void RemoveCard(const Card& card);
        private:
            std::vector<Card> cards;
            std::string username;
    };
}

#endif // PLAYER_HPP
