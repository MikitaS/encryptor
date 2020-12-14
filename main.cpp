#include <iostream>

#include "cpuid.h"
#include "encryptor.h"

int main() {
    std::cout
        << ">>"
        << hardware_identification::GetCPUId()
        << "<<" << std::endl;
    return 0;
}
