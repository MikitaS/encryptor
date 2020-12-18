#include <fstream>
#include <iostream>
#include <sstream>

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

std::string ReadFile(const std::string& file_name) {
    std::ifstream f(file_name);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

void WriteFile(const std::string& file_name, const std::string& text) {
    std::ofstream out(file_name);
    out << text;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "Invalid params" << std::endl;
        std::cout << "Usage: " << argv[0] << " <input file> <output file> \"crypt\"/\"decrypt\""
                  << std::endl;
        exit(0);
    }

    bool is_crypt;
    if (std::string(argv[3]) == "crypt") {
        is_crypt = true;
    } else if (std::string(argv[3]) == "decrypt") {
        is_crypt = false;
    } else {
        std::cout << "Invalid 3-rd argument" << std::endl;
        std::cout << "Usage: " << argv[0] << " <input file> <output file> \"crypt\"/\"decrypt\""
                  << std::endl;
        exit(0);
    }

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

    std::string plain_text;
    std::string crypted_text;
    // std::cout << pt << std::endl;

    if (is_crypt) {
        plain_text = ReadFile(argv[1]);
        CRITICAL_ENCRYPTOR_CHECK(
            encryptor::AbstractEncryptor::crypt(hash_key, plain_text, &crypted_text));

        WriteFile(argv[2], crypted_text);

    } else {
        crypted_text = ReadFile(argv[1]);
        CRITICAL_ENCRYPTOR_CHECK(encryptor::AbstractEncryptor::decrypt());

        WriteFile(argv[2], plain_text);
    }

    // std::cout << ct << std::endl;
    return 0;
}
