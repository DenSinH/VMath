# VMath
A wrapper for intel SSE/AVX vector instructions

This is just a toy thing to figure out what working with intrinsics is like. 
I tried to keep it kinda short, which was possible with some defines and templating.
Right now only basic operations are implemented (as far as possible),
so no strange other AVX/SSE functions. 

If you try to use an unimplemented / invalid operation 
(for example multiplication on unsigned vectors is unimplemented 
because no intrinsics exist, or shifting a floating point vector,
which is just invalid in general), it should compile to `nullptr()`, which
will cause the compiler to fail already.

I will probably add a matrix class later too.

[Intel instrinsics guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)
