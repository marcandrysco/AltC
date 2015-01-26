Memory Management
=================

AltC provides a set of memory management functions with enhanced functionality
for tracking memory usage. All memory allocations are tracked as a resource
and can be automatically freed using the [resource functions](res.html). For
test and debugging builds, all memory allocations are tracked to detect and
provide information about memory leaks.

## mem_alloc

### Synopsis

`void *mem_alloc(size_t nbytes)`

### Description

Allocate at least `nbytes` of memory. The memory should be manually freed by a
call to `free` when no longer needed. The allocation is added to the current
resource frame and will be automatically freed by a call to
[`res_clear`](res.html).

If the application is compiled in either debug or test mode, any unfreed
memory allocations are reported as warnings when the application terminates.

### Return Value

`mem_alloc` returns a pointer to the allocated memory.
