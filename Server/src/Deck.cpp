#include "Deck.hpp"
#include <algorithm>
#include <iostream>

using namespace JC9;

Deck::Deck() : drawnCards(), remainingCards()
{
    for (sf::Uint8 number(1); number < 14; number++)
    {
        for (sf::Uint8 type(0); type < Card::Type::Count; type++)
        {
            remainingCards.push_back(Card(static_cast<Card::Type>(type), number));
        }
    }
    Shuffle();
}

Card& Deck::PickACard()
{
    drawnCards.push_back(remainingCards.back());
    remainingCards.pop_back();
    return drawnCards.back();
}

void Deck::Shuffle()
{
    for (auto drawnCard : drawnCards)
        remainingCards.push_back(drawnCard);
    drawnCards.clear();
    std::random_shuffle(remainingCards.begin(), remainingCards.end());
}

unsigned int Deck::RemainingCards()const
{
    return remainingCards.size();
}
