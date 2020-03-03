=======================
Memory Allocator in C
=======================

Heap allocator that requests chunks of memory ``malloc`` or ``calloc`` from the operating
system and places several objects inside these. Using the ``free`` call
these memory objects can be freed up again, allowing for reuse by future
``malloc`` or ``calloc`` calls. ``realloc`` is also implemented to reallocate memory.

Standard library functions are not used, rather the allocator uses the 
``brk(2)`` and ``sbrk(2)`` functions, which ask the kernel for more heap space.

Created as part of the Operating Systems course at VU Amsterdam.

alloc.c is my code, the additional framework was provided.

Compiled with ``make`` and tested against the test library with ``make check``
