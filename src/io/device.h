#ifndef IO_DEVICE_H
#define IO_DEVICE_H

/*
 * null function declarations
 */

void io_null_close(void *ref);
bool io_null_ctrl(void *ref, unsigned int cmd, void *data);

#endif
