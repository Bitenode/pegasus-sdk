# Pegasus SDK

**Pegasus SDK** is the userspace sysroot for **Pegasus OS** — an upcoming hobby operating system for **x86_64**.

It is a relocatable cross-compile kit: headers, static libraries, a linker script, a Rust target spec, and a turnkey `pegasus-g++` wrapper so you can build freestanding C and C++ programs that run on Pegasus without pulling in a host libc.

> Pegasus OS is under active development. This SDK tracks the current userspace ABI (syscalls, capabilities, and static ELF layout). APIs may still change before a stable release.

## What's in the box

```
pegasus-sdk/
├── bin/
│   └── pegasus-g++          # Compile + link C++ against this sysroot in one step
├── include/
│   ├── *.h                  # POSIX-ish C library headers (stdio, unistd, pthread, …)
│   ├── sys/                 # sys/stat, sys/types, sys/time, …
│   ├── pegasus/             # OS-specific API
│   │   ├── syscall.h        # Syscall numbers + wrappers
│   │   ├── socket.h         # Minimal TCP client sockets
│   │   └── capability.h     # Process capability bitmasks
│   └── c++/14.2.0/          # GNU libstdc++ headers (C++17 STL)
├── lib/
│   ├── libc.a               # Pegasus C library
│   ├── libm.a               # Math
│   ├── libpthread.a         # Threads (backed by Pegasus syscalls)
│   ├── libcompiler_rt.a     # Compiler builtins
│   ├── libgcc.a             # GCC runtime / unwinder support
│   ├── libstdc++.a          # Full GNU libstdc++ (STL, iostream, filesystem, …)
│   ├── libsupc++.a          # C++ language support (exceptions, RTTI, …)
│   ├── pegasus.ld           # Linker script (static ELF, base 0x400000, W^X-friendly)
│   └── pegasus-cxx-runtime/ # operator new/delete + optional no-EH stub
├── share/
│   └── x86_64-unknown-pegasus.json   # Rust custom target spec
└── cpp-sdk.lock             # Optional SHA-256 lock for C++ archive integrity
```

### Highlights

| Piece | Role |
|-------|------|
| **C libc** | Freestanding, newlib-style static libc tailored to Pegasus syscalls |
| **C++ STL** | Cross-built GNU **libstdc++ 14.2.0** (`std::vector`, `std::string`, exceptions, threads, `<iostream>`, `std::filesystem`, …) |
| **`pegasus-g++`** | One-shot wrapper: encodes freestanding flags, includes, and the full-EH / no-EH link models |
| **`pegasus.ld`** | Static non-PIE layout at `0x400000` with page-aligned RX / RO / RW segments (matches kernel W^X) |
| **Rust target** | `x86_64-unknown-pegasus` JSON for `#![no_std]` / custom-target crates |
| **Capabilities** | Kernel capability bits (`CAP_FS_READ`, `CAP_NET_CONNECT`, …) exposed via `pegasus/capability.h` |

## Target model

- **Arch:** x86_64 (SysV ABI, small code model, no red zone)
- **Link:** static only (`-static -nostdlib`)
- **Entry:** `_start` via `pegasus.ld`
- **OS triple (Rust):** `x86_64-unknown-pegasus`
- **C/C++ toolchain:** typically `x86_64-elf-gcc` / `x86_64-elf-g++` with this directory as `--sysroot`

Programs are freestanding userspace ELFs loaded by the Pegasus kernel — not Linux or Windows binaries.

## Quick start (C++)

Requires an `x86_64-elf` cross GCC/binutils on your `PATH` (or set `PEGASUS_CXX` / `PEGASUS_LD`).

```sh
# Untar or clone this repo somewhere, then:
export PATH="/path/to/pegasus-sdk/bin:$PATH"

cat > hello.cpp <<'EOF'
#include <iostream>
int main() {
    std::cout << "hello from pegasus\n";
    return 0;
}
EOF

pegasus-g++ -o hello.elf hello.cpp
# -> static ELF linked at 0x400000
```

Useful flags:

- `--eh` / `--no-eh` — full exceptions+RTTI (default) vs abort-on-throw stub
- `--print-cmds` — show the exact compile/link lines
- `PEGASUS_SYSROOT` — override the auto-detected SDK root

## Quick start (C)

Point your cross compiler at this sysroot:

```sh
x86_64-elf-gcc -nostdinc -isystem /path/to/pegasus-sdk/include \
  -c hello.c -o hello.o

x86_64-elf-ld -nostdlib -static -m elf_x86_64 \
  -T /path/to/pegasus-sdk/lib/pegasus.ld \
  hello.o \
  /path/to/pegasus-sdk/lib/libc.a \
  /path/to/pegasus-sdk/lib/libm.a \
  /path/to/pegasus-sdk/lib/libcompiler_rt.a \
  -o hello.elf
```

## Quick start (Rust)

Use the bundled target spec:

```sh
rustc --target /path/to/pegasus-sdk/share/x86_64-unknown-pegasus.json ...
# Link against lib/libc.a (and friends) as needed for your crate
```

## Syscall surface (snapshot)

`include/pegasus/syscall.h` exposes the current ABI, including process/FS/IO (`exit`, `read`/`write`, `open`/`close`, `mmap`, `execve`, …), threads (`thread_create`, `futex`, …), and networking (`socket`, `connect`, `send`, `recv`). Numbers are generated from the OS `abi/syscalls.json` — treat this header as the source of truth for a given SDK revision.

## Status

| Area | Status |
|------|--------|
| C libc + core headers | Usable for userspace ports |
| GNU libstdc++ / `pegasus-g++` | Included in this tree |
| Rust target JSON | Included |
| Stable ABI / versioning | **Not frozen** — expect changes as Pegasus OS evolves |

This repository publishes the **assembled sysroot**. The sources that *build* it (kernel ABI generator, libc, cross-toolchain scripts) live in the main Pegasus OS tree.

## License

Licensing for Pegasus OS and redistributed components (GNU libstdc++, compiler-rt, etc.) will be clarified alongside the OS release. Third-party archives retain their upstream licenses (GPL/LGPL/Apache/etc. as applicable).
