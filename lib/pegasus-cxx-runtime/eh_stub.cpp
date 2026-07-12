// =============================================================================
// eh_stub.cpp — TEMPORARY abort-based C++ exception-handling stubs (Phase P4).
// =============================================================================
//
// The real GNU libstdc++.a / libsupc++.a we cross-build contain throwing code
// paths (e.g. std::vector::at, std::stoi, operator new failure) that reference
// the Itanium C++ ABI EH entry points (__cxa_throw, _Unwind_Resume,
// __gxx_personality_v0, ...) and the GCC unwinder. Pegasus programs in P4 are
// compiled `-fno-exceptions -fno-rtti`, so they never *raise* an exception, but
// the archive members that do get pulled in still need these symbols to *link*.
//
// This file provides minimal definitions that print a short diagnostic to
// stderr (fd 2) via write(2) and then abort(). Any actually-executed throw is
// therefore a clean, explained abort rather than undefined behaviour or a link
// failure.
//
// IMPORTANT: these are a P4 scaffold ONLY. Phase P5 (Itanium unwinder + real
// libsupc++ __cxa_*) REPLACES this whole file with libgcc_eh.a + the genuine
// libsupc++ exception runtime. Keep every EH stub isolated here so P5 can drop
// this single translation unit from the link line.
//
// Compile with the same freestanding C++ flags as the rest of userspace
// (-fno-exceptions -fno-rtti -nostdlib++); link the resulting object alongside
// new_delete.o and the libstdc++/libsupc++ archives. See docs/PORTING_CPP.md.
// =============================================================================

extern "C" {
#include <unistd.h>   // write
#include <stdlib.h>   // abort
#include <stddef.h>   // size_t
}

namespace {

// Write a fixed C string to stderr (fd 2) without touching stdio buffers.
inline void eh_msg(const char *s) {
    size_t n = 0;
    while (s[n]) ++n;
    (void)write(2, s, n);
}

// Common exit path for every stub: explain + abort. Marked noreturn so callers
// (and the throwing archive members) see a non-returning routine.
[[noreturn]] inline void eh_abort(const char *what) {
    eh_msg("pegasus: C++ exception path reached without an unwinder (");
    eh_msg(what);
    eh_msg("); aborting.\n");
    abort();
    for (;;) {}  // abort() is noreturn, but keep the analyzer happy.
}

}  // namespace

// -----------------------------------------------------------------------------
// Itanium C++ ABI exception entry points (normally from libsupc++).
// Signatures are ABI-compatible with the real ones; bodies just abort.
// -----------------------------------------------------------------------------
extern "C" {

// Allocate/free the exception object. Return type/args match libsupc++ so the
// throwing archive members resolve; the process never returns from a throw.
void *__cxa_allocate_exception(size_t) {
    eh_abort("__cxa_allocate_exception");
}

void __cxa_free_exception(void *) {
    eh_abort("__cxa_free_exception");
}

// The actual throw. Real signature: (void *thrown, std::type_info *, void(*)(void*)).
void __cxa_throw(void *, void *, void (*)(void *)) {
    eh_abort("__cxa_throw");
}

void __cxa_rethrow() {
    eh_abort("__cxa_rethrow");
}

void *__cxa_begin_catch(void *) {
    eh_abort("__cxa_begin_catch");
}

void __cxa_end_catch() {
    eh_abort("__cxa_end_catch");
}

// Pure-virtual call landing pad (referenced by vtables of abstract classes).
void __cxa_pure_virtual() {
    eh_abort("__cxa_pure_virtual");
}

// GCC unwinder resume; referenced by cleanup landing pads.
void _Unwind_Resume(void *) {
    eh_abort("_Unwind_Resume");
}

// The GNU C++ personality routine the unwinder would call. Return type is an
// _Unwind_Reason_Code (int) in the real ABI; we never return.
int __gxx_personality_v0(int, int, unsigned long long, void *, void *) {
    eh_abort("__gxx_personality_v0");
}

}  // extern "C"

// -----------------------------------------------------------------------------
// std::terminate — libstdc++ calls this on unrecoverable errors. Provide the
// mangled C++ symbol by defining it in namespace std.
// -----------------------------------------------------------------------------
namespace std {
[[noreturn]] void terminate() noexcept {
    eh_abort("std::terminate");
}
}  // namespace std
