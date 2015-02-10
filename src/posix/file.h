#ifndef POSIX_FILE_H
#define POSIX_FILE_H

/**
 * File flags enumerator.
 *   @_file_read_e: Read.
 *   @_file_write_e: Write.
 *   @_file_append_e: Append.
 *   @_file_create_e: Create.
 *   @_file_trunc_e: Truncate.
 */

enum _file_e {
	_file_read_e = 0x01,
	_file_write_e = 0x02,
	_file_append_e = 0x04,
	_file_create_e = 0x08,
	_file_trunc_e = 0x10
};

/*
 * file function declarations
 */

_file_t _open(const char *path, enum _file_e flags, ...);
void _close(_file_t file);

size_t _read(_file_t file, void *buf, size_t nbytes);
size_t _write(_file_t file, const void *buf, size_t nbytes);

/*
 * standard stream file definitions
 */

#define _file_stdout STDOUT_FILENO
#define _file_stderr STDERR_FILENO
#define _file_stdin STDIN_FILENO

#endif
