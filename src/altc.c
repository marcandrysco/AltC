#include "common.h"
#include "altc.h"
#include <stdio.h>
#include "posix/inc.h"
#include "io/output.h"
#include "io/input.h"
#include "mem.h"
#include "res.h"


/**
 * Initialize the AltC runtime.
 */

_export
void altc_init()
{
	_res_init();
	io_stdout = io_output_new(_file_stdout, 0);
	io_stderr = io_output_new(_file_stderr, 0);
	io_stdin = io_input_new(_file_stdin, 0);
}

/**
 * Destroy the AltC runtime.
 */

_export
void altc_destroy()
{
	io_output_close(io_stdout);
	io_output_close(io_stderr);
	io_input_close(io_stdin);
	_res_destroy();
}
