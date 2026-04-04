#include <stdint.h>
#include "utils/utils.h"
#include "serial/serial.h"

void kernel_main(uint64_t magic, void* bootInfo){
    serial_init();

    for (;;);
}