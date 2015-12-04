#ifndef CRYPTEDPACKET_HPP
#define CRYPTEDPACKET_HPP

#include <SFML/Network/Packet.hpp>

namespace JC9
{
    class CryptedPacket final : public sf::Packet
    {
        public:
            virtual const void* onSend(std::size_t& size)override;
            virtual void onReceive(const void* data, std::size_t size)override;
    };
}

#endif // CRYPTEDPACKET_HPP
