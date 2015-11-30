#include "Player.hpp"
#include <algorithm>

using namespace JC9;

void Player::AddCard(const Card& card)
{
    cards.push_back(card);
}

const std::vector<Card>& Player::GetCards() const
{
    return cards;
}


bool Player::HasCard(const Card& card)const
{
    return std::find(cards.begin(), cards.end(), card) != cards.end();
}

void Player::RemoveCard(const Card& card)
{
    auto it = std::find(cards.begin(), cards.end(), card);
    if (it != cards.end())
        cards.erase(it);
}
