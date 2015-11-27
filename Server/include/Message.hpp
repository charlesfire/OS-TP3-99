#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <SFML/Network/Packet.hpp>

namespace JC9
{
    class Message final
    {
        friend sf::Packet& operator >> (sf::Packet& packet, Message& message);
        public:
            enum Type : sf::Uint16 {};

            Message() = default;
            Message(Type type, const std::string& message);
            ~Message() = default;
            Type GetType()const;
            const std::string& GetMessage()const;
        private:
            std::string message;
            Type type;
    };

    sf::Packet& operator << (sf::Packet& packet, const Message& message)
    {
        return packet << message.GetType() << message.GetMessage();
    }

    sf::Packet& operator >> (sf::Packet& packet, Message& message)
    {
        sf::Uint16 type;
        packet >> type >> message.message;
        message.type = static_cast<Message::Type>(type);
        return packet;
    }
}

#endif // MESSAGE_HPP
