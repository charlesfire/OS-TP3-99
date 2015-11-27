#include "Message.hpp"

using namespace JC9;

Message::Message(Type type, const std::string& message) : message(message), type(type)
{

}

Message::Type Message::GetType()const
{
    return type;
}

const std::string& Message::GetMessage()const
{
    return message;
}

void Message::SetType(Type type)
{
    this->type = type;
}

void Message::SetMessage(const std::string& message)
{
    this->message = message;
}


sf::Packet& operator << (sf::Packet& packet, const Message& message)
{
    return packet << message.GetType() << message.GetMessage();
}

sf::Packet& operator >> (sf::Packet& packet, Message& message)
{
    sf::Uint16 type;
    std::string strMessage;
    packet >> type >> strMessage;
    message.SetType(static_cast<Message::Type>(type));
    message.SetMessage(strMessage);
    return packet;
}
