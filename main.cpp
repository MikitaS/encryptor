#include <iostream>

#include "cpuid.h"
#include "encryptor.h"

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
    std::vector<uint8_t> hash;

    PrintVectorUnsigned<uint32_t>(cpu_id, "HWID:");

    encryptor::AbstractEncryptor::hashgen(&hash, cpu_id);

    PrintVectorUnsigned<uint8_t>(hash, "HWID hash by SHA1:");

    return 0;
}
