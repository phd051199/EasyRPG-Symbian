#include <stdlib.h>

/* libc++ 4 emits an extern-template call for vector's length guard even
 * when exceptions are disabled. The S60 target does not ship libc++abi, so
 * terminate instead of pulling a second C++ runtime into EasyRPGScript. */
extern "C" void RgssVectorThrowLengthError()
    __asm__("_ZNKSt3__120__vector_base_commonILb1EE20__throw_length_errorEv");

extern "C" void RgssVectorThrowLengthError()
{
    abort();
}
