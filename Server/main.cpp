#include <cstdlib>
#include <iostream>
#include "Server.hpp"

std::unique_ptr<JC9::Server> server = nullptr;

int main()
{
    std::srand(52);
    sf::Uint16 port;
    std::cout << "Choose a port." << std::endl;
    std::cin >> port;
    server.reset(new JC9::Server(port));
    server->Run();
    return 0;
}
