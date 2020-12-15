#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <openssl/sha.h>

#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <cintrin>
#include <climits>
typedef unsigned __int32 uint32_t;
typedef unsigned __int8 uint8_t;

#else
#include <cstdint>
#endif

namespace encryptor {

enum Error { CORRECT, KEY_ERROR, DATA_ERROR };

class AbstractEncryptor {
   public:
    static encryptor::Error crypt();
    static encryptor::Error decrypt();

    static encryptor::Error hashgen(std::vector<uint8_t>* hash, const std::vector<uint32_t>& key);
    static encryptor::Error hashgen(std::vector<uint8_t>* hash, const std::string& key);
};

}  // namespace encryptor

#endif  // ENCRYPTOR_H