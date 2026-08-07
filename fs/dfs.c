#include <fs/dfs.h>

int check_dfs(void){
    superblock_t *sb = (superblock_t *)(ramdisk.start + DFS_BLOCKSIZE);
    if (sb->magic == DFS_MAGIC) return 1;
    return 0;
}