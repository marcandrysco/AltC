#ifndef FS_H
#define FS_H

/*
 * file system function declarations
 */

bool fs_exists(const char *path);

void fs_mkdir(struct io_chunk_t path);
void fs_mkdir_parents(const char *path);

#endif
