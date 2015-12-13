#include "Game.hpp"
#include <algorithm>
#include "Player.hpp"

using namespace JC9;

Game::Game() : deck(), players(), total(0)
{
    //ctor
}

Game::~Game()
{
    for (auto player : players)
        delete player;
    players.clear();
}

Player* Game::AddPlayer(const std::string& username, sf::Uint16 score)
{
    Player* player = new Player(username, score);
    for (unsigned int i(0); i < 3; i++)
        player->AddCard(deck.PickACard());
    players.push_back(player);
    return players.back();
}

bool Game::CanPlay(const Player* player)const
{
    for (auto card : player->GetCards())
    {
        if (CanPlayCard(card))
            return true;
    }
    return false;
}

bool Game::CanPlayCard(const Card& card)const
{
    switch (card.GetNumber())
    {
        case 1:
            return total + 1 <= 99;
        case 9:
            return true;
        case 10:
            return true;
        case 11:
            return true;
        case 12:
            return total + 10 <= 99;
        case 13:
            return true;
        default:
            return total + card.GetNumber() <= 99;
    }
}

const Player* Game::GetPlayingPlayer()const
{
    return players.front();
}

sf::Int16 Game::GetTotal()const
{
    return total;
}

Card Game::PickCard(const Player* player)
{
    if (deck.RemainingCards() == 0)
        deck.Shuffle();
    Card card = deck.PickACard();
    const_cast<Player*>(player)->AddCard(card);

    return card;
}

void Game::PlayCard(const Card& card)
{
    players.front()->RemoveCard(card);
    switch (card.GetNumber())
    {
        case 1:
            if (total + 11 < 99)
                total += 11;
            else
                total += 1;
            break;
        case 9:
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
            break;
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
}

void Game::RemovePlayer(const Player* player)
{
    auto it = std::find(players.begin(), players.end(), player);
    if (it != players.end())
    {
        players.erase(it);
        delete *it;
    }
}
