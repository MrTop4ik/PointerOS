#include <fs/fs.h>

ramdisk_t ramdisk;

void parse_ramdisk(unsigned int bootInfoAddr){
    struct multiboot_info* virtBootInfo = (struct multiboot_info *)(bootInfoAddr + KERNEL_OFFSET);

    struct multiboot_tag* tag = (struct multiboot_tag *)((uint8_t *)virtBootInfo + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END){
        if (tag->type == MULTIBOOT_TAG_TYPE_MODULE){
            struct multiboot_tag_module* module = (struct  multiboot_tag_module*)tag;
            if (memcmp(module->cmdline, "ramdisk", 7) == 0){
                ramdisk.type = module->type;
                ramdisk.size = module->size;
                ramdisk.start = module->mod_start;
                ramdisk.end = module->mod_end;
            }
            break;
        }
        tag = (struct multiboot_tag *)((uintptr_t)((uint8_t*)tag + tag->size + 7) & ~7);
    }
}