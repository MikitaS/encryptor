#include "encryptor.h"

#include <cassert>

namespace encryptor {

encryptor::Error AbstractEncryptor::hashgen(std::vector<uint8_t>* hash,
                                            const std::vector<uint32_t>& key) {
    assert(hash);
    if (key.size() < 2) {
        return Error::KEY_ERROR;
    }

    uint32_t tmp = 0;
    uint8_t* p_tmp = nullptr;
    std::vector<unsigned char> reinterpret_key;

    for (auto it = key.begin(); it != key.end(); ++it) {
        tmp = *it;
        p_tmp = reinterpret_cast<uint8_t*>(&tmp);
        reinterpret_key.push_back(static_cast<unsigned char>(p_tmp[0]));
        reinterpret_key.push_back(static_cast<unsigned char>(p_tmp[1]));
        reinterpret_key.push_back(static_cast<unsigned char>(p_tmp[2]));
        reinterpret_key.push_back(static_cast<unsigned char>(p_tmp[3]));
    }

    uint32_t vector_size = reinterpret_key.size();
    unsigned char* key_data = new unsigned char[vector_size];
    unsigned char* hash_data = new unsigned char[vector_size];

    int i = 0;
    for (auto it = reinterpret_key.begin(); it != reinterpret_key.end(); ++it, ++i) {
        key_data[i] = *it;
        hash_data[i] = 0;
    }

    SHA1(key_data, vector_size, hash_data);

    for (i = 0; i < vector_size; ++i) {
        if (hash_data[i] != 0) {
            hash->push_back(hash_data[i]);
        }
    }

    delete[] key_data;
    delete[] hash_data;
    return encryptor::Error::CORRECT;
}

encryptor::Error AbstractEncryptor::hashgen(std::vector<uint8_t>* hash, const std::string& key) {
    assert(hash);
    if (key.length() < 8) {
        return Error::KEY_ERROR;
    }

    uint32_t key_size = key.length();
    const unsigned char* key_data = reinterpret_cast<const unsigned char*>(key.c_str());
    unsigned char* hash_data = new unsigned char[key_size];

    for (int i = 0; i < key_size; ++i) {
        hash_data[i] = 0;
    }

    SHA1(key_data, key_size, hash_data);

    for (int i = 0; i < key_size; ++i) {
        if (hash_data[i] != 0) {
            hash->push_back(hash_data[i]);
        }
    }

    return encryptor::Error::CORRECT;
}

}  // namespace encryptor