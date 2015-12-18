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
            Player(const std::string& username, int score);
            ~Player() = default;
            void AddCard(const Card& card);
            const std::vector<Card>& GetCards()const;
            sf::Uint16 GetScore()const;
            const std::string& GetUsername()const;
            bool HasCard(const Card& card)const;
            void RemoveCard(const Card& card);
        private:
            std::vector<Card> cards;
            int score;
            std::string username;
    };
}

#endif // PLAYER_HPP
