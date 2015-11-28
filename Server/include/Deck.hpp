#ifndef DECK_HPP
#define DECK_HPP

#include <vector>
#include "Card.hpp"

namespace JC9
{
    class Deck final
    {
        public:
            Deck();
            Deck(const Deck& other) = delete;
            ~Deck() = default;
            Card& PickACard();
            void Shuffle();
            unsigned int RemainingCards()const;
        private:
            std::vector<Card> drawnCards;
            std::vector<Card> remainingCards;
    };
}

#endif // DECK_HPP
