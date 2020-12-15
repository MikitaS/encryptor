#include "password.h"

#include <cctype>

bool encryptor::CheckPasswordSafety(const std::string& password) {
    uint32_t length = password.length();
    if (length < 8) {
        return false;
    }

    bool special_exists = false;
    bool digit_exists = false;
    bool symbol_exists = false;
    bool uppercase_exists = false;

    for (int i = 0; i < length; ++i) {
        if (isdigit(password[i])) {
            digit_exists = true;
        } else if (isalpha(password[i])) {
            if (isupper(password[i])) {
                uppercase_exists = true;
            } else {
                symbol_exists = true;
            }
        } else if (ispunct(password[i])) {
            special_exists = true;
        }
    }

    if (!(symbol_exists && special_exists && digit_exists && uppercase_exists)) {
        return false;
    }

    return true;
}