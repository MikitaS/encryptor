#include "cpuid.h"

std::string hardware_identification::GetCPUId() {
    std::string hwid_cpu;
    hardware_identification::CPUID cpu_id(
        static_cast<int>(false));
    hwid_cpu += std::string(
        (const char*)&cpu_id.EAX(), 4);
    hwid_cpu += std::string(
        (const char*)&cpu_id.EBX(), 4);
    hwid_cpu += std::string(
        (const char*)&cpu_id.EDX(), 4);
    hwid_cpu += std::string(
        (const char*)&cpu_id.ECX(), 4);
    return hwid_cpu;
}