#include "Encryptor.hpp"

using namespace JC9;

std::string Encryptor::Crypt(const std::string& data)
{
    std::string cryptedData;
    for (auto c : data)
        cryptedData.push_back(c - 34);

    return cryptedData;
}

std::string Encryptor::Decrypt(const std::string& data)
{
    std::string decryptedData;
    for (auto c : data)
        decryptedData.push_back(c + 34);

    return decryptedData;
}
