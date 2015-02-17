#ifndef POSIX_SYS_H
#define POSIX_SYS_H

/*
 * time function declarations
 */

int64_t _time(void);
int64_t _utime(void);

void _sleep(uint64_t sec);
void _usleep(uint64_t usec);

int64_t _clock_realtime();
int64_t _clock_monotonic();

#endif
