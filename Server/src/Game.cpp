#include "Game.hpp"
#include <algorithm>

using namespace JC9;

Game::Game() : deck(), players(), total(0)
{
    //ctor
}

Game::~Game()
{
    players.clear();
}

Player* Game::AddPlayer()
{
    Player player;
    for (unsigned int i(0); i < 3; i++)
        player.AddCard(deck.PickACard());
    players.push_back(player);
    return &players.back();
}

bool Game::CanPlayCard(const Card& card)const
{
    switch (card.GetNumber())
    {
        case 1:
            return total + 1 < 99;
        case 10:
            return true;
        case 11:
            return true;
        case 12:
            return total + 10 < 99;
        case 13:
            return true;
        default:
            return total + card.GetNumber() < 99;
    }
}

unsigned int Game::GetTotal()const
{
    return total;
}

bool Game::PlayACard(const Card& card)
{
    if (players.front().HasCard(card))
        players.front().RemoveCard(card);
    else
        return false;

    players.front().AddCard(deck.PickACard());

    switch (card.GetNumber())
    {
        case 1:
            if (total + 11 < 99)
                total += 11;
            else
                total += 1;
            break;
        case 10:
            if (total + 10 > 99)
                total -= 10;
            else
                total += 10;
            break;
        case 11:
            players.push_back(players.front());
            players.erase(players.begin());
            std::reverse(players.begin(), players.end());
        case 12:
            total += 10;
            break;
        case 13:
            total = 99;
            break;
        default:
            total += card.GetNumber();
            break;
    }

    players.push_back(players.front());
    players.erase(players.begin());

    if (deck.RemainingCards() == 0)
        deck.Shuffle();

    return true;
}
