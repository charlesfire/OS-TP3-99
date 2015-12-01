#pragma once
#include <SFML/Network.hpp>
#include "Card.hpp"
#include "Constant.h"

namespace JC9
{
	class Game
	{
	private:
		bool canPlay = false;
		Card* lastPlayed;
		sf::Int16 totalOnGame = 0;
		sf::TcpSocket host;
		sf::TcpSocket::Status status = sf::Socket::Done;

		Card* cartesEnMain[NB_CARTES];

		void ReactToTransaction(sf::Packet& packet);
		void AskForCard(sf::Packet& packet);
		std::string ConvertToString(sf::Uint8 type);

	public:
		Game();
		~Game();
		void Run();

	};
}

