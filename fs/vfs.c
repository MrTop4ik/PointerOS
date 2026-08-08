#include <fs/fs.h>

vfs_ops_t dfs_ops = {.read = dfs_read, .finddir = dfs_finddir};

vfs_node_t *mount_ramdisk(void){
    if (!check_dfs()){
        serial_print("[FS] Invalid FS\n");
        for(;;);
    }

    superblock_t *sb = (superblock_t *)(ramdisk.start + DFS_BLOCKSIZE);

    vfs_node_t *root_node = (vfs_node_t *)kmalloc(sizeof(vfs_node_t));
    if (!root_node) return NULL;

    dfs_vfs_ctx_t *root_ctx = (dfs_vfs_ctx_t *)kmalloc(sizeof(dfs_vfs_ctx_t));
    if (!root_ctx){
        kfree(root_node);
        return NULL;
    }

    inode_t *ctx_node = (inode_t *)(ramdisk.start + (sb->inode_table_block * DFS_BLOCKSIZE) + sizeof(inode_t));

    root_ctx->inode = ctx_node;
    root_ctx->inode_num = 1;
    
    root_node->priv_data = root_ctx;
    root_node->name[0] = '/'; root_node->name[1] = '\0';
    root_node->ops = &dfs_ops;
    root_node->size = ctx_node->size;
    root_node->type = ctx_node->type;

    serial_print("[FS] Successfully mounted DFS\n");
    return root_node;
}