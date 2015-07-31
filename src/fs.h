#ifndef FS_H
#define FS_H

/*
 * file system variables
 */

struct io_chunk_t fs_tmpchunk;

/*
 * file system function declarations
 */

bool fs_exists(const char *path);

bool fs_isdir(const char *path);
bool fs_isdirf(const char *restrict format, ...);
bool fs_isdirfv(const char *restrict format, va_list args);

void fs_mkdir(const char *path);

void fs_mkdir_parents(const char *path);
void fs_mkdirf_parents(const char *restrict format, ...);
void fs_mkdirfv_parents(const char *restrict format, va_list args);

void fs_remove(const char *path);
void fs_remove_recurse(const char *path);

char *fs_tmpname(const char *prefix);

void fs_writef(const char *restrict path, const char *restrict format, ...);

char *fs_basename(const char *path);
struct io_chunk_t fs_dirname(const char *path);
size_t fs_dirname_len(const char *path);

#endif
