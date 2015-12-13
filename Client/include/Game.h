#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "Card.hpp"
#include "Constant.h"
#include "CryptedPacket.hpp"

namespace JC9
{
	class Game
	{
	private:
		bool canPlay = false;
		bool gameIsOver = false;
		bool isIdentify = false;
		Card* lastPlayed;
		sf::Int16 totalOnGame = 0;
		sf::TcpSocket host;
		sf::TcpSocket::Status status = sf::Socket::Done;
		

		Card* cartesEnMain[NB_CARTES];

		void ReactToTransaction(CryptedPacket& packet);
		void AskForCard(CryptedPacket& packet);
		std::string ConvertToString(sf::Uint8 type);

	public:
		Game();
		~Game();
		void Run();

	};
}

