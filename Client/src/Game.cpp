#include "Game.h"
#include <iostream>
#include <string>
#include "MessageType.hpp"

using namespace JC9;

Game::Game()
{
	canPlay = false;
	totalOnGame = 0;
	host.setBlocking(true);
	lastPlayed = nullptr;
	for (int i = 0; i < NB_CARTES; i++)
	{
		cartesEnMain[i] = nullptr;
	}
}


Game::~Game()
{
	for (int i = 0; i < NB_CARTES; i++)
	{
		delete cartesEnMain[i];
	}
	delete lastPlayed;
}

void Game::Run()
{

	sf::TcpSocket::Status status = host.connect("10.17.59.194", 6666);

	while (true)
	{
		if (status != sf::TcpSocket::Status::Done)
		{
			break;
		}
		system("cls");
		sf::Packet packet;
		status = host.receive(packet);
		ReactToTransaction(packet);
		if (canPlay)
		{
			AskForCard(packet);
		}
		
	}
	host.disconnect();
}
void Game::AskForCard(sf::Packet& packet)
{
	
	std::cout << "Le total des cartes au centre: " << static_cast<int>(totalOnGame) << std::endl;
	std::cout << "Voici vos cartes: " << std::endl;
	//On affiche les cartes que le joueur a dans ses mains.
	for (int i = 0; i < NB_CARTES; i++)
	{
		if (cartesEnMain[i] != nullptr)
		{
			std::cout << i + 1 << ". " << static_cast<int>(cartesEnMain[i]->GetNumber()) << " de " << ConvertToString(cartesEnMain[i]->GetType()) << std::endl;
		}
	}

	bool isValid = false;
	int numCarte = 0;
	//On s'assure qu'il n'ait pas entré de la junk.
	std::cout << std::endl << "Veuillez entrez le numéro de la carte que vous voulez jouer" << std::endl;
	while (!isValid)
	{
		std::cin >> numCarte;
		if (numCarte >= 1 && numCarte <= NB_CARTES)
		{
			isValid = true;
		}
		else
		{
			std::cout << "Vous avez entré un mauvais numéro, veuillez réessayer." << std::endl;
		}
	}
	sf::Packet response;
	response << MessageType::CardSelected;
	response << *cartesEnMain[numCarte - 1];
	//Puis on envoie une transaction au serveur.

	status = host.send(response);
	if (lastPlayed != nullptr)
	{
		//On enlève la carte des mains du joueur.
		delete lastPlayed;
		lastPlayed = cartesEnMain[numCarte - 1];
	}
	cartesEnMain[numCarte - 1] = nullptr;
	//Ensuite ce n'est plus à son tour et il doit attendre l'autorisation du serveur pour jouer à nouveau.
	canPlay = false;


}
std::string Game::ConvertToString(sf::Uint8 type)
{
	switch (type)
	{
	case Card::Diamond:
	{
		return "carreau";
	}
	case Card::Club:
	{
		return "pique";
	}
	case Card::Heart:
	{
		return "coeur";
	}
	default:
		return "trèfles";
	}
}
void Game::ReactToTransaction(sf::Packet& packet)
{
	MessageType type;
	packet >> type;
	switch (type)
	{
	case MessageType::ConnectionSucceeded:
	{
		canPlay = true;
		break;
	}
	case MessageType::CardPicked:
	{
		for (int i = 0; i < NB_CARTES; i++)
		{
			if (cartesEnMain[i] == nullptr)
			{
				
				Card carte;
				packet >> carte;
				cartesEnMain[i] = new Card(carte);
				break;

			}
		}
		break;
	}
	case MessageType::GameFinished:
	{
		std::string message = "";
		packet >> message;
		if (message == "V")
		{
			std::cout << "Vous avez gagnez!" << std::endl;
		}
		else
		{
			std::cout << "Vous avez perdu!" << std::endl;
		}
		break;

	}
	case MessageType::CardPlayed:
	{
		Card carte;
		packet >> carte;
		std::string name;
		//TO DO...
		packet >> totalOnGame;
		std::cout << "Nouveau total: " << totalOnGame << std::endl;
		std::cout << "Une carte à été jouée: " << std::endl << static_cast<int>(carte.GetNumber()) << " de " << ConvertToString(carte.GetType()) << std::endl;
		break;
	}
	case MessageType::YourTurn:
	{
		canPlay = true;
		break;
	}
	case MessageType::InvalidCard:
	{
		canPlay = true;
		std::cout << "Veuillez sélectionner une carte valide!" << std::endl;
	}
	default:
		break;
	}
}