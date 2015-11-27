#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <SFML/Network/Packet.hpp>

namespace JC9
{
    class Message final
    {
        public:
            enum Type : sf::Uint16 {};

            Message() = default;
            Message(Type type, const std::string& message);
            ~Message() = default;
            Type GetType()const;
            const std::string& GetMessage()const;
            void SetType(Type type);
            void SetMessage(const std::string&);
        private:
            std::string message;
            Type type;
    };
}

sf::Packet& operator << (sf::Packet& packet, const JC9::Message& message);

sf::Packet& operator >> (sf::Packet& packet, JC9::Message& message);

#endif // MESSAGE_HPP
