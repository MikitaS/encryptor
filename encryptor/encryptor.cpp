#include "encryptor.h"

#include <cassert>
#include <list>

namespace encryptor {

constexpr uint32_t SHA_MAX_LENGHT = SHA_DIGEST_LENGTH * sizeof(int);

encryptor::Error AbstractEncryptor::crypt(const std::vector<uint8_t>& key, const std::string& pt,
                                          std::string* ct) {
    struct Column {
        uint8_t num = 0;
        std::vector<char> letters;
    };

    uint32_t keySize = key.size();
    uint32_t ptSize = pt.size();
    uint32_t clsNum = (ptSize < keySize ? ptSize : keySize);
    if (clsNum) {
        return encryptor::Error::DATA_ERROR;
    }

    std::vector<Column*> cls;

    for (uint32_t i = 0; i < clsNum; ++i) {
        Column* tmp = new Column;
        tmp->num = key[i];
        cls.push_back(tmp);
    }

    uint32_t cur = 0;
    for (auto it : pt) {
        cls[cur]->letters.push_back(it);
        cur++;
        if (cur == clsNum) {
            cur = 0;
        }
    }

    std::list<Column*> myList(cls.begin(), cls.end());
    myList.sort([](Column* x, Column* y) { return x->num < y->num; });

    for (auto* i : myList) {
        for (auto j : i->letters) {
            ct->push_back(j);
        }
    }

    for (auto* i : cls) {
        delete i;
    }

    return encryptor::Error::CORRECT;
}
encryptor::Error AbstractEncryptor::decrypt() { return encryptor::Error::CORRECT; }

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
    unsigned char* hash_data = new unsigned char[SHA_MAX_LENGHT];

    int i = 0;
    for (auto it = reinterpret_key.begin(); it != reinterpret_key.end(); ++it, ++i) {
        key_data[i] = *it;
    }

    for (int i = 0; i < SHA_MAX_LENGHT; ++i) {
        hash_data[i] = 0;
    }

    SHA1(key_data, vector_size, hash_data);

    for (i = 0; i < SHA_MAX_LENGHT; ++i) {
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
    unsigned char* hash_data = new unsigned char[SHA_MAX_LENGHT];

    for (int i = 0; i < SHA_MAX_LENGHT; ++i) {
        hash_data[i] = 0;
    }

    SHA1(key_data, key_size, hash_data);

    for (int i = 0; i < SHA_MAX_LENGHT; ++i) {
        if (hash_data[i] != 0) {
            hash->push_back(hash_data[i]);
        }
    }

    delete[] hash_data;

    return encryptor::Error::CORRECT;
}

}  // namespace encryptor