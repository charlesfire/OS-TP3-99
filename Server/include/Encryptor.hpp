#ifndef ENCRYPTOR_HPP
#define ENCRYPTOR_HPP

#include <string>

namespace JC9
{
    class Encryptor final
    {
        public:
            Encryptor() = delete;
            virtual ~Encryptor() = delete;
            static std::string Crypt(const std::string& data);
            static std::string Decrypt(const std::string& data);
    };
}

#endif // ENCRYPTOR_HPP
