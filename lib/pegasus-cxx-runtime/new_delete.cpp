// Freestanding C++ operator new/delete backed by Pegasus libc malloc/free.
//
// This one file is compiled into THREE distinct link contexts (see the root
// Makefile). The allocation-failure policy is guarded on the __EXCEPTIONS macro
// (GCC/Clang define it iff exceptions are enabled) because only one of those
// contexts has a C++ exception runtime to throw into:
//
//   1. Freestanding demos (USER_CXXFLAGS: -fno-exceptions -nostdlib++, no
//      libsupc++). __EXCEPTIONS is undefined. There is no EH runtime and no
//      <new> on the include path, so a failed allocation MUST return NULL — it
//      cannot throw.
//   2. No-EH STL demo cpp_stl (STL_CXXFLAGS: -fno-exceptions). __EXCEPTIONS is
//      undefined. libsupc++ IS linked, but so is eh_stub.o (abort stubs), and
//      this object is linked before libsupc++ so its symbols win. We keep the
//      malloc-forwarding operators here so a failed allocation returns NULL
//      instead of throwing std::bad_alloc straight into the abort stubs.
//   3. Full-EH demos (EXC_CXXFLAGS: -fexceptions). __EXCEPTIONS is defined. The
//      genuine libsupc++ + libgcc_eh are linked. We deliberately define NOTHING
//      in this branch and let libsupc++ supply the standard-conformant
//      operators: throwing operator new/new[] (std::bad_alloc on failure) plus
//      the C++17 nothrow and over-aligned overloads, all with the correct
//      type_info so `catch (std::bad_alloc&)` works. Re-defining them here would
//      need <new> (not on this TU's include path — the STL builds compile this
//      file with only the libc include dir) or a hand-rolled std::bad_alloc
//      whose type_info would not match libsupc++'s.
//
// So: under -fno-exceptions we provide the full non-throwing surface; under
// -fexceptions we get out of libsupc++'s conformant way.

#ifndef __EXCEPTIONS

#include <stdint.h>
extern "C" {
#include <stdlib.h>
}

// Minimal std declarations for the C++17 nothrow / over-aligned overload
// signatures. This TU is compiled without the C++ standard headers on its
// include path (freestanding uses -nostdinc++; the STL builds pass only the
// libc include dir for this file), so we spell out just enough of <new> to form
// the mangled operator names. These are the ABI-standard shapes, so the emitted
// symbols match what the compiler generates at every call site.
namespace std {
struct nothrow_t {};
enum class align_val_t : size_t {};
}  // namespace std

// -- Over-aligned allocation shim -------------------------------------------
// libc's aligned_alloc only satisfies alignments up to the 16-byte malloc
// granularity, so back over-aligned new with a manual shim: over-allocate, then
// stash the exact malloc() base immediately below the returned aligned pointer
// so the matching delete can free() that exact base.
static void *aligned_allocate(size_t size, size_t alignment) {
    if (alignment < sizeof(void *))
        alignment = sizeof(void *);
    const size_t header = sizeof(void *);
    if (size > SIZE_MAX - alignment - header)
        return nullptr;  // rounding up would overflow
    void *base = malloc(size + alignment - 1 + header);
    if (!base)
        return nullptr;
    uintptr_t raw = (uintptr_t)base + header;
    uintptr_t aligned = (raw + (alignment - 1)) & ~(uintptr_t)(alignment - 1);
    ((void **)aligned)[-1] = base;
    return (void *)aligned;
}

static void aligned_release(void *ptr) noexcept {
    if (ptr)
        free(((void **)ptr)[-1]);
}

// -- Plain new / delete ------------------------------------------------------
void *operator new(size_t size) { return malloc(size); }
void *operator new[](size_t size) { return malloc(size); }

void operator delete(void *ptr) noexcept { free(ptr); }
void operator delete[](void *ptr) noexcept { free(ptr); }
void operator delete(void *ptr, size_t) noexcept { free(ptr); }
void operator delete[](void *ptr, size_t) noexcept { free(ptr); }

// -- nothrow new / delete ----------------------------------------------------
void *operator new(size_t size, const std::nothrow_t &) noexcept { return malloc(size); }
void *operator new[](size_t size, const std::nothrow_t &) noexcept { return malloc(size); }
void operator delete(void *ptr, const std::nothrow_t &) noexcept { free(ptr); }
void operator delete[](void *ptr, const std::nothrow_t &) noexcept { free(ptr); }

// -- Over-aligned new / delete (C++17) --------------------------------------
void *operator new(size_t size, std::align_val_t al) {
    return aligned_allocate(size, (size_t)al);
}
void *operator new[](size_t size, std::align_val_t al) {
    return aligned_allocate(size, (size_t)al);
}
void *operator new(size_t size, std::align_val_t al, const std::nothrow_t &) noexcept {
    return aligned_allocate(size, (size_t)al);
}
void *operator new[](size_t size, std::align_val_t al, const std::nothrow_t &) noexcept {
    return aligned_allocate(size, (size_t)al);
}

void operator delete(void *ptr, std::align_val_t) noexcept { aligned_release(ptr); }
void operator delete[](void *ptr, std::align_val_t) noexcept { aligned_release(ptr); }
void operator delete(void *ptr, size_t, std::align_val_t) noexcept { aligned_release(ptr); }
void operator delete[](void *ptr, size_t, std::align_val_t) noexcept { aligned_release(ptr); }
void operator delete(void *ptr, std::align_val_t, const std::nothrow_t &) noexcept { aligned_release(ptr); }
void operator delete[](void *ptr, std::align_val_t, const std::nothrow_t &) noexcept { aligned_release(ptr); }

#endif  // !__EXCEPTIONS
