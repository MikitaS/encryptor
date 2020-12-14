#include "cpuid.h"

#include <cpuid.h>

#include <iostream>

std::vector<uint32_t> hardware_identification::GetHWID() {
    return hardware_identification::GetCPUId();
}

std::vector<uint32_t> hardware_identification::GetCPUId() {
    std::vector<uint32_t> id_key;

    unsigned eax = 1, ebx = 0, ecx = 0, edx = 0;

    hardware_identification::native_cpuid(&eax, &ebx, &ecx, &edx);

    id_key.push_back(eax & 0xF);
    id_key.push_back((eax >> 4) & 0xF);
    id_key.push_back((eax >> 8) & 0xF);
    id_key.push_back((eax >> 12) & 0x3);
    id_key.push_back((eax >> 16) & 0xF);
    id_key.push_back((eax >> 20) & 0xFF);
    id_key.push_back(ecx);
    id_key.push_back(edx);

    eax = 0x16;
    native_cpuid(&eax, &ebx, &ecx, &edx);

    id_key.push_back(ebx);
    id_key.push_back(ecx);
    id_key.push_back(edx);

    return id_key;
}