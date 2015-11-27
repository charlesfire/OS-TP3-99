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
