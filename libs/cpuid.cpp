#include "cpuid.h"

#include <iostream>

#define REGISTER_CALL_DEPTH 0x18

std::vector<uint32_t>
hardware_identification::GetCPUId() {
    std::vector<uint32_t> id_key;

    for (int i = 5; i < REGISTER_CALL_DEPTH;
         i++) {

        std::cout << i <<")" << std::endl;
        hardware_identification::CPUID cpu_id(i);

        id_key.push_back(cpu_id.EDX());
        id_key.push_back(cpu_id.ECX());
        std::cout <<cpu_id.EDX() << " " << cpu_id.ECX() << " " << std::endl << std::endl;
    }
    return id_key;
}