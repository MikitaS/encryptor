#include <iostream>

#include "cpuid.h"
#include "encryptor.h"

int main() {
    std::vector<uint32_t> cpu_id = hardware_identification::GetHWID();

    for (auto it = cpu_id.begin(); it != cpu_id.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;

    return 0;
}
