#ifndef GAME_HPP
#define GAME_HPP

#include "Deck.hpp"

namespace JC9
{
    class Game
    {
        public:
            Game();
            ~Game() = default;
            bool CanPlayCard(const Card& card)const;
            unsigned int GetPlayingPlayer()const;
            unsigned int GetTotal()const;
            Card& PlayACard(const Card& card);
        private:
            enum PlayingOrder {Clockwise = 1, CounterClockwise = -1};

            Deck deck;
            PlayingOrder playingOrder;
            unsigned int playingPlayer;
            unsigned int total;
    };
}

#endif // GAME_HPP
