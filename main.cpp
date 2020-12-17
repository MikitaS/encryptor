#include <iostream>

#include "cpuid.h"
#include "encryptor.h"
#include "password.h"

template <typename T>
void PrintVectorUnsigned(std::vector<T> vctr, const std::string& comment) {
    std::cout << comment << std::endl;
    for (auto it = vctr.begin(); it != vctr.end(); ++it) {
        std::cout << static_cast<unsigned int>(*it) << ' ';
    }
    std::cout << std::endl;
}

int main() {
    std::vector<uint32_t> cpu_id = hardware_identification::GetHWID();
    std::vector<uint8_t> hash_cpu;
    std::vector<uint8_t> hash_key;
    std::string str_password;

    std::cout << "Please, create a password." << std::endl;
    std::cin >> str_password;

    while (true) {
        if (encryptor::CheckPasswordSafety(str_password)) {
            std::cout << "Processing..." << std::endl;
            break;
        } else {
            std::cout << "Unsafe password, please create new" << std::endl;
            std::cout << "Password must contain more than 8 characters, at least:\n-- 1 digit\n-- "
                         "1 downcase symbol\n-- 1 uppercase symbol\n-- 1 special symbol"
                      << std::endl;
            std::cout << "New password:\n>>";
            std::cin >> str_password;
        }
    }

    PrintVectorUnsigned<uint32_t>(cpu_id, "HWID:");

    CRITICAL_ENCRYPTOR_CHECK(encryptor::AbstractEncryptor::hashgen(&hash_cpu, cpu_id));
    PrintVectorUnsigned<uint8_t>(hash_cpu, "HWID hash by SHA1:");

    CRITICAL_ENCRYPTOR_CHECK(encryptor::AbstractEncryptor::hashgen(&hash_key, str_password));
    PrintVectorUnsigned<uint8_t>(hash_key, "password hash by SHA1:");

    const std::string pt =
        "Hello, world! Password must contain more than 8 characters, at least:! We loop over all "
        "sections in the ELF object. Function elf nextscn wil";
    std::string ct;
    encryptor::AbstractEncryptor::crypt(hash_key, pt, &ct);
    std::cout << ct << std::endl;
    encryptor::AbstractEncryptor::decrypt();

    return 0;
}
