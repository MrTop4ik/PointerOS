#include <fs/fs.h>

int check_dfs(void){
    superblock_t *sb = (superblock_t *)(ramdisk.start + DFS_BLOCKSIZE);
    if (sb->magic == DFS_MAGIC) return 1;
    return 0;
}

int dfs_read(struct vfs_node *node, uint32_t offset, size_t size, uint8_t *buffer){
    if (!node || !size || !buffer || node->type != DFS_TYPE_FILE) return -1;

    dfs_vfs_ctx_t *ctx = (dfs_vfs_ctx_t *)node->priv_data;

    if (offset > (ctx->inode->extents[0].length) * DFS_BLOCKSIZE) return -1;
    else if (offset == ((ctx->inode->extents[0].length) * DFS_BLOCKSIZE)) return 0;

    uint8_t *start_block = (uint8_t *)(ramdisk.start + (ctx->inode->extents[0].start_block * DFS_BLOCKSIZE));

    if (size + offset > (ctx->inode->extents[0].length * DFS_BLOCKSIZE)) size = (ctx->inode->extents[0].length * DFS_BLOCKSIZE) - offset;

    memcpy(buffer, start_block, size);

    
    return size;
}

struct vfs_node* dfs_finddir(struct vfs_node *node, const char *name){

}