#include "Game.hpp"

using namespace JC9;

Game::Game() : deck(), playingOrder(PlayingOrder::Clockwise), playingPlayer(0), total(0)
{
    //ctor
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

unsigned int Game::GetPlayingPlayer()const
{
    return playingPlayer;
}

unsigned int Game::GetTotal()const
{
    return total;
}

Card& Game::PlayACard(const Card& card)
{
    switch (card.GetNumber())
    {
        case 1:
            if (total + 11 < 99)
                total += 11;
            else
                total += 1;
        case 10:
            if (total + 10 > 99)
                total -= 10;
            else
                total += 10;
        case 11:
            playingOrder = static_cast<PlayingOrder>(-playingOrder);
        case 12:
            total += 10;
        case 13:
            total = 99;
        default:
            total += card.GetNumber();
            break;
    }

    playingPlayer = (playingPlayer + playingOrder) % 4;

    if (deck.RemainingCards() == 0)
        deck.Shuffle();
    return deck.PickACard();
}
