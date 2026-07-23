# Pegasus SDK

**Pegasus SDK** is the userspace sysroot for **Pegasus OS** — a hobby operating
system for **x86_64**.

It is a relocatable cross-compile kit: headers, static libraries, linker script,
compile drivers, examples, and documentation so you can build freestanding C
and C++ programs (including windowed GUI apps) without a host libc.

> Pegasus OS is under active development. This SDK tracks the current userspace
> ABI. APIs may still change before a stable release.

## What's in the box

```
pegasus-sdk/
├── bin/
│   ├── pegasus-cc           # freestanding C compile+link wrapper
│   └── pegasus-g++          # freestanding C++ wrapper (when STL is bundled)
├── docs/
│   ├── README.md            # this file (also at sysroot root)
│   ├── GUI_SDK.md           # windowed apps (libgui) + gui_hello example
│   └── ABI.md               # syscall + capability reference
├── examples/
│   └── gui_hello.c          # basic windowed app (same as in-tree gui_hello)
├── include/
│   ├── *.h                  # POSIX-ish C library headers
│   ├── sys/                 # sys/stat, sys/types, sys/shm, …
│   ├── pegasus/             # OS-specific API
│   │   ├── syscall.h        # syscall numbers + raw syscallN() wrappers
│   │   ├── socket.h         # minimal TCP client
│   │   ├── capability.h     # process capability bitmasks
│   │   ├── window.h         # libgui window API
│   │   ├── gui_event.h      # input events
│   │   ├── blit.h, font.h   # ARGB + bitmap font helpers
│   └── c++/14.2.0/          # GNU libstdc++ headers (when bundled)
├── lib/
│   ├── libc.a, libm.a, libpthread.a, libgui.a, libcompiler_rt.a
│   ├── libstdc++.a, libsupc++.a, libgcc*.a  (optional C++ bundle)
│   ├── pegasus.ld           # linker script (base 0x400000)
│   └── pegasus-cxx-runtime/
├── share/
│   └── x86_64-unknown-pegasus.json   # Rust custom target spec
└── cpp-sdk.lock             # optional SHA-256 lock for C++ archives
```

## Quick start (C — console)

```sh
export PATH="/path/to/pegasus-sdk/bin:$PATH"

cat > hello.c <<'EOF'
#include <stdio.h>
int main(void) {
    puts("Hello from Pegasus!");
    return 0;
}
EOF

pegasus-cc -o hello.elf hello.c
```

## Quick start (C — windowed GUI)

The reference app is **gui_hello** (full walkthrough in
[docs/GUI_SDK.md](GUI_SDK.md)). It must be launched on Pegasus with the
**GUI spawn profile** (Start → **GUI Hello**, or Terminal `spawn gui`).

```sh
export PATH="/path/to/pegasus-sdk/bin:$PATH"
pegasus-cc -o gui_hello.elf examples/gui_hello.c -lgui
```

That source draws centered text, redraws on maximize/resize, and quits on
window close, Escape, or `q`. See [GUI_SDK.md](GUI_SDK.md) for the complete
listing and API tables.

## Quick start (C++)

Requires `x86_64-elf-g++` on `PATH` (or `PEGASUS_CXX` / `PEGASUS_LD`).

```sh
pegasus-g++ -o hello.elf hello.cpp
```

Flags: `--eh` / `--no-eh`, `--print-cmds`, `PEGASUS_SYSROOT`.

## Quick start (Rust)

```sh
rustc --target /path/to/pegasus-sdk/share/x86_64-unknown-pegasus.json ...
```

Link against `lib/libc.a` (and friends) as needed.

## Target model

| Item | Value |
|------|-------|
| Arch | x86_64 SysV, small code model, no red zone |
| Link | static only (`-static -nostdlib`) |
| Entry | `_start` via `pegasus.ld` at `0x400000` |
| Toolchain | `x86_64-elf-gcc` / `clang` + `ld.lld` |
| Hosts | Linux, macOS, WSL (no native Windows toolchain in-tree) |

## Documentation

| Doc | Contents |
|-----|----------|
| [docs/GUI_SDK.md](GUI_SDK.md) | libgui API, **gui_hello** example, events, GUI spawn |
| [docs/ABI.md](ABI.md) | syscall table, capabilities, profiles |
| `include/pegasus/syscall.h` | authoritative syscall numbers |
| `examples/gui_hello.c` | copy-paste starting point for windowed apps |

## Highlights

| Piece | Role |
|-------|------|
| **libc** | freestanding static C library (stdio, unistd, signals, pthread subset, …) |
| **libgui** | windowed apps via SHM surfaces + `WINDOW_*` syscalls |
| **libstdc++** | optional GNU STL 14.2 (`pegasus-g++`) |
| **pegasus-cc / pegasus-g++** | one-shot compile+link drivers |
| **Capabilities** | kernel-enforced access (`CAP_FS_*`, `CAP_NET_*`, `CAP_DISPLAY`, …) |

## Status

| Area | Status |
|------|--------|
| C libc + coreutils-style ports | usable |
| GUI / libgui | usable (GUI spawn required on target; see gui_hello) |
| C++ STL / pegasus-g++ | bundled when cross-built |
| Rust target JSON | included |
| Stable ABI | **not frozen** |

Sources that build this sysroot live in the main Pegasus OS tree
(`userspace/`, `abi/syscalls.json`, cross-toolchain scripts).

Rebuild/install from the OS tree:

```sh
make sdk-install SDK_DESTDIR=/path/to/pegasus-sdk
```
