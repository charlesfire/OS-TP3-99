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
		sf::Uint8 totalOnGame = 0;
		sf::TcpSocket host;
		sf::TcpSocket::Status status = sf::Socket::Done;

		sf::Packet packet;
		Card* cartesEnMain[NB_CARTES];

		void ReactToTransaction();
		void AskForCard();
		std::string ConvertToString(sf::Uint8 type);

	public:
		Game();
		~Game();
		void Run();

	};
}

