#include "Game.h"
#include <iostream>
#include <string>

using namespace JC9;

Game::Game()
{
	canPlay = true;
	totalOnGame = 0;
	for (int i = 0; i < NB_CARTES; i++)
	{
		cartesEnMain[i] = "";
	}
}


Game::~Game()
{
}

void Game::Run()
{
	//sf::TcpSocket::Status status = host.connect("10.17.59.194", 6666);

	while (true)
	{
		//status = host.receive(packet);
		ReactToTransaction(6);
		if (canPlay)
		{
			AskForCard();
		}
		if (status != sf::TcpSocket::Status::Done)
		{
			break;
		}
	}
	host.disconnect();
}
void Game::AskForCard()
{
	std::cout << "Voici vos cartes: " << std::endl;
	//On affiche les cartes que le joueur a dans ses mains.
	for (int i = 0; i < NB_CARTES; i++)
	{
		if (cartesEnMain[i] != "")
		{
			std::cout <<  i + 1<< ". " << cartesEnMain[i] << std::endl;
		}
	}

	bool isValid = false;
	int numCarte = 0;
	//On s'assure qu'il n'ait pas entré de la junk.
	while (!isValid)
	{
		std::cout << std::endl << "Veuillez entrez le numéro de la carte que vous voulez jouer" << std::endl;
		
		std::cin >> numCarte;
		if (numCarte >= 1 && numCarte <= NB_CARTES)
		{
			isValid = true;
		}
	}
	
	//Puis on envoie une requête au serveur.
	message.SetType(Message::EnvoieCarte);
	message.SetMessage(";" + cartesEnMain[numCarte - 1]);
	host.send(packet);
	cartesEnMain[numCarte - 1] = "";
	//Ensuite ce n'est plus à son tour et il doit attendre l'autorisation du serveur pour jouer à nouveau.
	canPlay = false;


}
void Game::ReactToTransaction(int number)
{
	switch (number)
	{
	case 1:
	{
		canPlay = true;
	}
	case 2:
	{
		for (int i = 0; i < NB_CARTES; i++)
		{
			if (cartesEnMain[i] == "")
			{
				packet >> message;
				cartesEnMain[i] = message.GetMessage();
				break;

			}
		}
	}
	case 4:
	{
		packet >> message;
		std::cout << message.GetMessage() << std::endl;

	}
	case 6:
	{
		std::string text = message.GetMessage();
		packet >> message;
		std::string value = text.substr(text.find_first_of(';') + 1, text.length());
		totalOnGame = std::stoi(value);
	}
	case 8:
	{
		canPlay = true;
	}
	default:
		break;
	}
}