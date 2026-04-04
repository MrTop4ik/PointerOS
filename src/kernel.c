#include <stdint.h>
#include "utils/utils.h"
#include "serial/serial.h"
#include "idt/idt.h"
#include "pit.h"

void kernel_main(uint64_t magic, void* bootInfo){
    serial_init();
    init_IDT();
    init_PIT(10);

    for (;;);
}