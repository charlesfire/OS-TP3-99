#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include "Deck.hpp"
#include "Player.hpp"

namespace JC9
{
    class Game final
    {
        public:
            Game();
            ~Game();
            Player* AddPlayer();
            bool CanPlay(const Player* player)const;
            bool CanPlayCard(const Card& card)const;
            const Player* GetPlayingPlayer()const;
            unsigned int GetTotal()const;
            bool PlayCard(const Card& card);
        private:
            Deck deck;
            std::vector<Player> players;
            unsigned int total;
    };
}

#endif // GAME_HPP
