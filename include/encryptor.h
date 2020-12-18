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

#define CRITICAL_ENCRYPTOR_CHECK(EXPRESSION)                                                       \
    switch (EXPRESSION) {                                                                          \
        case encryptor::Error::CORRECT:                                                            \
            break;                                                                                 \
        case encryptor::Error::KEY_ERROR:                                                          \
            std::cerr << "SOMETHING WRONG WITH KEY.\n FILE: " << __FILE__ << " LINE: " << __LINE__ \
                      << "\nAborting process.." << std::endl;                                      \
            abort();                                                                               \
        case encryptor::Error::DATA_ERROR:                                                         \
            std::cerr << "SOMETHING WRONG WITH DATA. \n FILE: " << __FILE__                        \
                      << " LINE: " << __LINE__ << "\nAborting process.." << std::endl;             \
            abort();                                                                               \
        default:                                                                                   \
            break;                                                                                 \
    }

namespace encryptor {

enum Error { CORRECT, KEY_ERROR, DATA_ERROR };

class AbstractEncryptor {
   public:
    static encryptor::Error crypt(const std::vector<uint8_t>& key, const std::string& pt,
                                  std::string* ct);
    static encryptor::Error decrypt(const std::vector<uint8_t>& key, const std::string& ct,
                                    std::string* pt);

    static encryptor::Error hashgen(std::vector<uint8_t>* hash, const std::vector<uint32_t>& key);
    static encryptor::Error hashgen(std::vector<uint8_t>* hash, const std::string& key);
};

}  // namespace encryptor

#endif  // ENCRYPTOR_H