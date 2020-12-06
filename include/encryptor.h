#pragma once

#include <iostream>
#include <string>

namespace encryptor {

enum error { CORRECT, KEY_ERROR, DATA_ERROR };

class AbstractEncryptor {
public:
  static encryptor::error crypt(/*params here*/);
  static encryptor::error decrypt(/*params here*/);
  static encryptor::error keygen(/*params here*/);
};

} // namespace encryptor
