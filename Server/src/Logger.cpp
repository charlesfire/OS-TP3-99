#include "Logger.hpp"

using namespace JC9;

void Logger::Log(const std::string& message)
{
    (*out) << message;
}

void Logger::SetOutput(std::ostream& out)
{
    this->out = &out;
}
