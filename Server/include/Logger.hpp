#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include "Singleton.h"

namespace JC9
{
    class Logger final : public Singleton<Logger>
    {
        public:
            Logger();
            void Log(const std::string& message);
        private:
            std::ofstream out;

            std::string GetDate()const;
    };
}

#endif // LOGGER_HPP
