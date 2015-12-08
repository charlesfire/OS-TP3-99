#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include "Singleton.h"

namespace JC9
{
    class Logger final : public Singleton<Logger>
    {
        public:
            void Log(const std::string& message);
            void SetOutput(std::ostream& out = std::cerr);
        private:
            std::ostream* out;
    };
}

JC9::Logger& operator<<(JC9::Logger& os, const std::string& message)
{
    os.Log(message);
    return os;
}

#endif // LOGGER_HPP
