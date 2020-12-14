#ifndef CPUID_H
#define CPUID_H

#include <string>

#ifdef _WIN32
#include <intrin.h>
#include <limits.h>
typedef unsigned __int32 uint32_t;

#else
#include <stdint.h>
#endif
namespace hardware_identification {

std::string GetCPUId();

class CPUID {
   public:
    explicit CPUID(unsigned i) {
#ifdef _WIN32
        __cpuid((int*)regs, (int)i);

#else
        asm volatile(
            "cpuid"
            : "=a"(regs[0]), "=b"(regs[1]),
              "=c"(regs[2]), "=d"(regs[3])
            : "a"(i), "c"(0));
#endif
    }

    const uint32_t& EAX() const {
        return regs[0];
    }
    const uint32_t& EBX() const {
        return regs[1];
    }
    const uint32_t& ECX() const {
        return regs[2];
    }
    const uint32_t& EDX() const {
        return regs[3];
    }

   private:
    uint32_t regs[4];
};

static inline void native_cpuid(
    unsigned int* eax, unsigned int* ebx,
    unsigned int* ecx, unsigned int* edx) {
    /* ecx is often an input as well as an output.
     */
    asm volatile("cpuid"
                 : "=a"(*eax), "=b"(*ebx),
                   "=c"(*ecx), "=d"(*edx)
                 : "0"(*eax), "2"(*ecx));
}

}  // namespace hardware_identification

#endif  // CPUID_H