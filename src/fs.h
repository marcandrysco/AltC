#ifndef FS_H
#define FS_H

/*
 * file system function declarations
 */

bool fs_exists(const char *path);

void fs_mkdir(const char *path);
void fs_mkdir_parents(const char *path);

void fs_remove(const char *path);
void fs_remove_recurse(const char *path);

void fs_writef(const char *restrict path, const char *restrict format, ...);

#endif
