#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>
#include "Singleton.h"

namespace JC9
{
    class Logger final : public Singleton<Logger>
    {
        template<class T>
        friend Logger& operator<<(Logger& logger, const T& message)
        {
            logger.Log(message);
            return logger;
        }

        public:
            Logger(const std::string& path = "Log.txt");

            Logger& operator<<(std::ostream& (*F)(std::ostream&));

            static std::string GetDate();

            template<class T>
            void Log(const T& message)
            {
                out << message;
                std::cout << message;
            }
        private:
            std::ofstream out;
    };
}

#endif // LOGGER_HPP
