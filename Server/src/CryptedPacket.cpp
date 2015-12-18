#include "CryptedPacket.hpp"
#include <string>

using namespace JC9;

const void* CryptedPacket::onSend(std::size_t& size)
{
    sf::Uint8* data = static_cast<sf::Uint8*>(const_cast<void*>(getData()));
    size = getDataSize();
    for (unsigned int i(0); i < size; i++)
    {
        sf::Uint8 total = 0;
        for (unsigned int j(0); j < 4; j++)
        {
            sf::Uint8 b1 = data[i] << (j * 2);
            sf::Uint8 b2 = data[i] << (j * 2 + 1);
            b1 = b1 >> 7;
            b2 = b2 >> 7;

            total = total | (b2 << ((3 - j) * 2 + 1)) | (b1 << ((3 - j) * 2));
        }

        data[i] = total;
    }

    return data;
}

void CryptedPacket::onReceive(const void* data, std::size_t size)
{
    sf::Uint8* decryptedData = static_cast<sf::Uint8*>(const_cast<void*>(data));
    for (unsigned int i(0); i < size; i++)
    {
        sf::Uint8 total = 0;
        for (unsigned int j(0); j < 4; j++)
        {
            sf::Uint8 b1 = decryptedData[i] << (j * 2);
            sf::Uint8 b2 = decryptedData[i] << (j * 2 + 1);
            b1 = b1 >> 7;
            b2 = b2 >> 7;

            total = total | (b2 << ((3 - j) * 2 + 1)) | (b1 << ((3 - j) * 2));
        }

        decryptedData[i] = total;
    }

    append(decryptedData, size);
}
