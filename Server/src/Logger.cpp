#include "Logger.hpp"
#include <ctime>
#include <iomanip>

using namespace JC9;

Logger::Logger(const std::string& path) : out(path)
{

}

Logger& Logger::operator<<(std::ostream& (* F)(std::ostream&))
{
    out << F;
    std::cout << F;
    return *this;
}


std::string Logger::GetDate()
{
    std::stringstream sstream;
    std::time_t t = time(nullptr);
    sstream << std::put_time(std::localtime(&t), "%c %Z");
    return sstream.str();
}
