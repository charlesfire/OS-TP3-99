#pragma once
#include <SFML/Network.hpp>
#include "Message.hpp"
#include "Constant.h"

namespace JC9
{
	class Game
	{
	private:
		bool canPlay = false;
		int totalOnGame = 0;
		Message message;
		sf::TcpSocket host;
		sf::TcpSocket::Status status = sf::Socket::Done;

		sf::Packet packet;
		std::string cartesEnMain[NB_CARTES];

		void ReactToTransaction(int number);
		void AskForCard();

	public:
		Game();
		~Game();
		void Run();

	};
}

