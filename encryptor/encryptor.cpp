#include "encryptor.h"

#include <algorithm>
#include <cassert>
#include <list>

namespace encryptor {

constexpr uint32_t SHA_MAX_LENGHT = SHA_DIGEST_LENGTH * sizeof(int);

encryptor::Error AbstractEncryptor::crypt(const std::vector<uint8_t>& key, const std::string& pt,
                                          std::string* ct) {
    uint32_t keySize = key.size();
    uint32_t ptSize = pt.size();
    uint32_t clsNum = (ptSize < keySize ? ptSize : keySize);
    uint32_t curColumn = 0;
    uint32_t curLetter = 0;

    if (!clsNum) {
        std::cerr << "Size of the key or of the input text is zero" << std::endl;
        return encryptor::Error::DATA_ERROR;
    }

    std::vector<Column*> cls;

    for (uint32_t i = 0; i < clsNum; ++i) {
        Column* cl = new Column;
        cl->num = key[i];
        cls.push_back(cl);
    }

    // hack to place source text in a rectangular table
    std::string plainText = pt;
    for (uint32_t i = 0; i < (clsNum - ptSize % clsNum); i++) {
        plainText.push_back('\0');
    }

    curColumn = 0;
    for (auto it : plainText) {
        cls[curColumn]->data.push_back(it);
        curColumn++;
        curColumn %= clsNum;
    }

    std::sort(cls.begin(), cls.end(), [](Column* x, Column* y) { return x->num < y->num; });

    curColumn = 0;
    curLetter = 0;
    for (uint32_t i = 0; i < plainText.size(); ++i) {
        ct->push_back(cls[curColumn]->data.at(curLetter));
        curColumn++;
        curColumn %= clsNum;
        if (curColumn == 0) {
            curLetter++;
        }
    }

    for (auto* i : cls) {
        delete i;
    }

    return encryptor::Error::CORRECT;
}

encryptor::Error AbstractEncryptor::decrypt(const std::vector<uint8_t>& key, const std::string& ct,
                                            std::string* pt) {
    uint32_t keySize = key.size();
    uint32_t ctSize = ct.size();
    uint32_t clsNum = (ctSize < keySize ? ctSize : keySize);
    uint32_t curColumn = 0;
    uint32_t curLetter = 0;

    if (clsNum == 0) {
        std::cerr << "Size of the key or of the input text is zero" << std::endl;
        return encryptor::Error::DATA_ERROR;
    }

    std::vector<Column*> cls;

    // Generate partKey if the whole key wasn't used in the encryption
    std::vector<uint8_t> partKey;
    for (uint32_t i = 0; i < clsNum; ++i) {
        partKey.push_back(key[i]);
    }
    std::sort(partKey.begin(), partKey.end(), [](uint8_t x, uint8_t y) { return x < y; });

    for (uint32_t i = 0; i < clsNum; ++i) {
        Column* cl = new Column;
        cl->num = partKey[i];
        cls.push_back(cl);
    }

    curColumn = 0;
    for (auto it : ct) {
        cls[curColumn]->data.push_back(it);
        curColumn++;
        curColumn %= clsNum;
    }

    std::vector<Column*> decrCls;
    for (uint32_t i = 0; i < clsNum; ++i) {
        for (auto* it : cls) {
            if (it->num == key[i] && it->isViewed == false) {
                decrCls.push_back(it);
                it->isViewed = true;
                break;
            }
        }
    }

    curColumn = 0;
    curLetter = 0;
    for (uint32_t i = 0; i < ctSize; ++i) {
        pt->push_back(decrCls[curColumn]->data.at(curLetter));
        curColumn++;
        curColumn %= clsNum;
        if (curColumn == 0) {
            curLetter++;
        }
    }
    for (auto* i : cls) {
        delete i;
    }
    return encryptor::Error::CORRECT;
}

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