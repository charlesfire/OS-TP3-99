#include "Game.h"
#include <iostream>
#include <string>
#include "MessageType.hpp"


using namespace JC9;

Game::Game()
{
	canPlay = false;
	gameIsOver = false;
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
	std::string user;
	std::string password;
	std::string ipAddress;
	std::string port;

	std::cout << "Veuillez entrer le port où vous souhaitez vous connecter." << std::endl;
	std::cin >> port;
	std::cout << "Entrez l'adresse IP du serveur où vous voulez vous connecter: " << std::endl;
	std::cin >> ipAddress;
	std::cout << "Connexion au serveur en cours, veuillez patienter." << std::endl;

	sf::TcpSocket::Status status = host.connect(ipAddress, std::stoi(port));
	std::cout << "Veuillez entrer votre nom  d'utilisateur: " << std::endl;
	std::cin >> user;
	std::cout << "Veuillez entrer votre mot de passe: " << std::endl;
	std::cin >> password;

	CryptedPacket connection;
	connection << MessageType::Connection;
	connection << user;
	connection << password;
	status = host.send(connection);


	while (!gameIsOver)
	{
		if (status != sf::TcpSocket::Status::Done)
		{
			break;
		}
		std::cout << "En attente d'une action d'un autre joueur..." << std::endl;
		CryptedPacket packet;
		status = host.receive(packet);
		system("cls");

		ReactToTransaction(packet);

		if (canPlay)
		{
			AskForCard(packet);
		}		
	}

	host.disconnect();
}
void Game::AskForCard(CryptedPacket& packet)
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
			numCarte = 0;
		}
	}
	CryptedPacket response;
	response << MessageType::CardSelected;
	response << *cartesEnMain[numCarte - 1];

	//Puis on envoie une transaction au serveur.
	status = host.send(response);
	//On garde en mémoire la dernière carte jouée par le joueur au cas où elle n'était pas valide.
	lastPlayed = cartesEnMain[numCarte - 1];
	cartesEnMain[numCarte - 1] = nullptr;
	//Ensuite ce n'est plus à son tour et il doit attendre l'autorisation du serveur pour jouer à nouveau.
	canPlay = false;


}
/// <summary>
/// Converti le type enuméré de la carte en string.
/// </summary>
/// <param name="type">The type.</param>
/// <returns></returns>
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
		return "trèfles";
	}
	case Card::Heart:
	{
		return "coeur";
	}
	default:
		return "pique";
	}
}
/// <summary>
/// Réagit à une transaction reçu par le serveur.
/// </summary>
/// <param name="packet">The packet.</param>
void Game::ReactToTransaction(CryptedPacket& packet)
{
	MessageType type;
	packet >> type;
	switch (type)
	{
	case MessageType::ConnectionSucceeded:
	{
		std::cout << "Connexion réussie!" << std::endl;
		system("Pause");
		break;
	}
	case MessageType::ConnectionFailed:
	{
		std::cout << "Connexion échouée!" << std::endl;
		system("Pause");
		gameIsOver = true;
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
				std::cout << "Vous avez reçu le " << static_cast<int>(carte.GetNumber()) << " de " << ConvertToString(carte.GetType()) << std::endl;
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
		system("Pause");
		break;

	}
	case MessageType::CardPlayed:
	{
		if (lastPlayed != nullptr)
		{
			delete lastPlayed;
			lastPlayed = nullptr;
		}
		Card carte;
		packet >> carte;
		std::string name;
		//TO DO...
		packet >> totalOnGame;
		std::cout << "Nouveau total: " << totalOnGame << std::endl;
		std::cout << "Une carte à été jouée: " << std::endl << static_cast<int>(carte.GetNumber()) << " de " << ConvertToString(carte.GetType()) << std::endl;
		system("Pause");
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
		for (int i = 0; i < NB_CARTES; i++)
		{
			if (cartesEnMain[i] == nullptr)
			{
				cartesEnMain[i] = lastPlayed;
				break;
			}
		}
		std::cout << "Veuillez sélectionner une carte valide!" << std::endl;
		break;
	}
	case MessageType::Leaderboard:
	{
		gameIsOver = true;
		std::cout << "Voici le leaderboard actuel: " << std::endl;
		for (int i = 0; i < 5; i++)
		{
			std::string name;
			packet >> name;
			sf::Uint16 score;
			packet >> score;
			std::cout << i + 1 << ". " << name << ": " << score << " points" << std::endl;
		}
		system("Pause");

	}
	default:
	{
		std::cout << "Type inconnu!" << std::endl;
		std::cout << std::endl;
		break;
	}
	}
}