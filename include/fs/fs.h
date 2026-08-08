#pragma once
#include <stdint.h>
#include <multiboot2.h>
#include <libc/string.h>
#include <arch/x86_64/drivers/video/serial.h>
#include <arch/x86_64/inlineasm.h>
#include <mm/vmalloc.h>

#define DFS_MAGIC        0x00534644
#define DFS_BLOCKSIZE    0x1000
#define DFS_MAX_FILENAME 56

#define DFS_TYPE_FREE 0
#define DFS_TYPE_FILE 1
#define DFS_TYPE_DIR  2

typedef struct {
    uint32_t type;
    uint32_t size;
    uint64_t start;
    uint64_t end;
} ramdisk_t;

typedef struct {
    uint32_t logical_block;
    uint32_t length;
    uint64_t start_block;
}__attribute__((packed)) extent_t;

typedef struct {
    uint64_t size;
    uint16_t type;
    uint16_t links_count;
    uint32_t flags;
    extent_t extents[4];
    uint8_t reserved[48];
}__attribute__((packed)) inode_t;

typedef struct {
    uint64_t magic;
    uint32_t block_size;
    uint32_t total_blocks;
    uint32_t total_inodes;
    uint32_t free_blocks;
    uint32_t free_inodes;
    uint32_t block_bitmap_block;
    uint32_t inode_bitmap_block;
    uint32_t inode_table_block;
    uint32_t data_blocks_start;
}__attribute__((packed)) superblock_t;

typedef struct {
    uint32_t inode_num;
    uint8_t type;
    uint8_t reserved[3];
    char name[DFS_MAX_FILENAME];
}__attribute__((packed)) dir_entry_t;

typedef struct {
    uint32_t inode_num;
    inode_t *inode;
} dfs_vfs_ctx_t;

struct vfs_node;

typedef struct {
    int (*read)(struct vfs_node *node, uint32_t offset, size_t size, uint8_t *buffer);
    struct vfs_node* (*finddir)(struct vfs_node *node, const char *name);
} vfs_ops_t;

typedef struct vfs_node {
    char name[DFS_MAX_FILENAME];
    uint8_t type;
    uint32_t size;
    vfs_ops_t *ops;
    void* priv_data;
} vfs_node_t;

extern ramdisk_t ramdisk;

void parse_ramdisk(unsigned int bootInfoAddr);

vfs_node_t *mount_ramdisk(void);

int check_dfs(void);
int dfs_read(struct vfs_node *node, uint32_t offset, size_t size, uint8_t *buffer);
struct vfs_node* dfs_finddir(struct vfs_node *node, const char *name);