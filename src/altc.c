#include "common.h"
#include "rt.h"
#include <stdio.h>
#include "posix/inc.h"
#include "io/output.h"
#include "mem.h"
#include "res.h"


_export
void altc_init()
{
	_res_init();
	io_stdout = io_output_new(_file_stdout, 0);
	io_stderr = io_output_new(_file_stderr, 0);
}


_export
void altc_destroy()
{
	io_output_close(io_stdout);
	io_output_close(io_stderr);
	_res_destroy();
}
