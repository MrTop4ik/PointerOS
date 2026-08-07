#pragma once
#include <stdint.h>
#include <multiboot2.h>
#include <libc/string.h>
#include <arch/x86_64/drivers/video/serial.h>
#include <arch/x86_64/inlineasm.h>
#include <mm/vmalloc.h>
#include <fs/dfs.h>

struct vfs_node;

typedef struct {
    int (*read)(struct vfs_node *noed, uint32_t offset, size_t size, uint8_t *buffer);
    struct vfs_node* (*finddir)(struct vfs_node *node, const char *name);
} vfs_ops_t;

typedef struct vfs_node {
    char name[DFS_MAX_FILENAME];
    uint8_t type;
    uint32_t size;
    vfs_ops_t *ops;
    void* priv_data;
} vfs_node_t;

vfs_node_t *mount_ramdisk(void);