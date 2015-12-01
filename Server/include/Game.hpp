#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include "Deck.hpp"

namespace JC9
{
    class Player;

    class Game final
    {
        public:
            Game();
            ~Game();
            Player* AddPlayer();
            bool CanPlay(const Player* player)const;
            bool CanPlayCard(const Card& card)const;
            const Player* GetPlayingPlayer()const;
            sf::Int16 GetTotal()const;
            Card PickCard(const Player* player);
            void PlayCard(const Card& card);
        private:
            Deck deck;
            std::vector<Player*> players;
            sf::Int16 total;
    };
}

#endif // GAME_HPP
