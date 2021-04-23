#include "random.hpp"

extern void __check_cpu_random_support() asm("__check_cpu_random_support");
extern void __cpu_random_generate() asm("__cpu_random_generate");
extern bool __cpu_random_support_var asm("__cpu_random_support_var");
extern uint64_t __cpu_random_value asm("__cpu_random_value");
extern bool __cpu_random_value_success asm("__cpu_random_value_success");

namespace Kernel {

bool hasCPUTrueRandom() {
    __check_cpu_random_support();
    return __cpu_random_support_var;
}
uint64_t getCPUTrueRandom() {
    __cpu_random_generate();
    if(__cpu_random_value_success) {
        return __cpu_random_value;
    } else {
        return 0;
    }
}

}