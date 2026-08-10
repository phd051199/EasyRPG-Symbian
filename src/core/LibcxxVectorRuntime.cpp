#include <vector>

// libc++ 4.0.1 normally emits this explicit instantiation from locale.cpp.
// The Symbian core does not use the locale subsystem, so instantiate only the
// shared vector bounds helpers instead of pulling the entire locale runtime.
namespace std {
inline namespace __1 {
template class __vector_base_common<true>;
}
}
