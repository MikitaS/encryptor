#include "cpuid.h"

#define REGISTER_CALL_DEPTH 6

std::vector<uint32_t>
hardware_identification::GetCPUId() {
    std::vector<uint32_t> id_key;

    for (int i = 0; i < REGISTER_CALL_DEPTH;
         i++) {
        hardware_identification::CPUID cpu_id(i);

        id_key.push_back(cpu_id.EBX());
        id_key.push_back(cpu_id.EDX());
        id_key.push_back(cpu_id.ECX());
    }
    return id_key;
}