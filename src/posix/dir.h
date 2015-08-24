#ifndef POSIX_DIR_H
#define POSIX_DIR_H

/*
 * directory function declarations
 */

struct io_chunk_t _cwd(void);
void _chdir(const char *dir);

size_t _userdir(char *str, size_t nbytes);
struct io_chunk_t _userdirchunk();

#endif
