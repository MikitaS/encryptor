#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <iostream>
#include <string>

namespace encryptor {

enum Error { CORRECT, KEY_ERROR, DATA_ERROR };

class AbstractEncryptor {
   public:
    static encryptor::Error crypt(
        /*params here*/);
    static encryptor::Error decrypt(
        /*params here*/);
    static encryptor::Error keygen(
        /*params here*/);
};

}  // namespace encryptor

#endif  // ENCRYPTOR_H