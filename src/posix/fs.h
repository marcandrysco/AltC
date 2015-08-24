#ifndef POSIX_FS_H
#define POSIX_FS_H

/*
 * file system function declarations
 */

bool _exists(const char *path);
bool _isfile(const char *path);
bool _islink(const char *path);
bool _isdir(const char *path);
void _mkdir(const char *path);

void _remove(const char *path);

struct _fsiter_t _fsiter_init(const char *path);
void _fsiter_destroy(struct _fsiter_t *iter);
const char *_fsiter_next(struct _fsiter_t *iter, bool *dir);

#endif
