#include <iostream>
#include "Server.hpp"

int main()
{
    sf::Uint16 port;
    std::cout << "Choose a port." << std::endl;
    std::cin >> port;
    JC9::Server server(port);
    server.Run();
    return 0;
}
