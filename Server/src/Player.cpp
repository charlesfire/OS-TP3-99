#include "Player.hpp"
#include <algorithm>

using namespace JC9;

Player::Player(const std::string& username, int score) : cards(), username(username), score(score)
{

}

void Player::AddCard(const Card& card)
{
    cards.push_back(card);
}

const std::vector<Card>& Player::GetCards() const
{
    return cards;
}

const std::string& Player::GetUsername() const
{
    return username;
}

sf::Uint16 Player::GetScore() const
{
    return score;
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
