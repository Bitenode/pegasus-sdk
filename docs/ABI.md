# Pegasus SDK — userspace ABI reference

This document summarizes the syscall surface and conventions exposed by this
sysroot. Treat `include/pegasus/syscall.h` as the source of truth for numbers
on a given SDK revision.

## Conventions

- **Return values:** syscalls return a `long`. Success is ≥ 0 (or 0 for void-like
  ops). Errors are negative errno values (`-EPERM`, `-EINVAL`, …). See
  `include/errno.h`.
- **Link model:** static ELF only, base `0x400000`, entry `_start` via
  `lib/pegasus.ld`. Freestanding — no host libc.
- **Capabilities:** spawned processes carry a capability mask checked by the
  kernel. See `include/pegasus/capability.h`.

## Capability bits

| Bit | Name | Typical use |
|-----|------|-------------|
| 0 | `CAP_FS_READ` | read/open/stat paths |
| 1 | `CAP_FS_WRITE` | write/create/unlink/mkdir/rename |
| 2 | `CAP_NET_CONNECT` | TCP client (`socket`/`connect`/`send`/`recv`) |
| 3 | `CAP_DISPLAY` | `WINDOW_*` syscalls (GUI apps) |
| 4 | `CAP_INPUT` | paired with display for GUI input delivery |

CLI spawns from `psh` receive `CAP_FS_READ | CAP_FS_WRITE | CAP_NET_CONNECT`.
GUI spawns add `CAP_DISPLAY | CAP_INPUT`.

## Syscall table

`SYSCALL_MAX` = **48** (numbers are stable; new syscalls append only).

| nr | Name | libc / notes |
|----|------|--------------|
| 0 | `EXIT` | `_exit()` |
| 1 | `READ` | `read()` |
| 2 | `WRITE` | `write()` |
| 3 | `OPEN` | `open()` |
| 4 | `CLOSE` | `close()` |
| 5 | `MMAP` | `mmap()` |
| 6 | `MUNMAP` | `munmap()` |
| 7 | `YIELD` | `yield()` |
| 8 | `GETPID` | `getpid()` |
| 9 | `BRK` | `sbrk()` |
| 10 | `IOCTL` | reserved (often `-ENOSYS`) |
| 11 | `WAIT` | `waitpid()` |
| 12 | `KILL` | `kill()` |
| 13 | `GETCAPS` | `getcaps()` |
| 14 | `SLEEP` | `usleep()` |
| 15–16 | `THREAD_CREATE` / `THREAD_EXIT` | `thread_create()` / `thread_exit()` |
| 17 | `GETTID` | `gettid()` |
| 18 | `LSEEK` | `lseek()` |
| 19 | `EXECVE` | `execve()` / `spawn()` |
| 20–23 | socket ops | `socket()`, `connect()`, `send()`, `recv()` |
| 24–25 | `STAT` / `FSTAT` | `stat()`, `fstat()` |
| 26 | `GETTIMEOFDAY` | `gettimeofday()` |
| 27 | `GETDENTS` | directory listing |
| 28–35 | FS ops | `unlink`, `mkdir`, `rename`, `pipe`, `dup2`, `chdir`, `getcwd`, `rmdir` |
| 36 | `FUTEX` | `futex_wait()` / `futex_wake()` |
| 37–39 | signals | `sigaction`, `__sigreturn`, `sigprocmask` |
| 40 | `SHM_CREATE` | `shm_create(name, size)` — see `sys/shm.h` |
| 41 | `SHM_MAP` | `shm_map(id)` |
| 42 | `WINDOW_CREATE` | libgui `pg_window_create` — requires `CAP_DISPLAY` |
| 43 | `WINDOW_DESTROY` | libgui `pg_window_destroy` |
| 44 | `WINDOW_SET_TITLE` | title update |
| 45 | `WINDOW_SET_GEOM` | geometry update |
| 46 | `WINDOW_PRESENT` | libgui `pg_window_present` |
| 47 | `WINDOW_POLL_EVENT` | libgui `pg_window_poll` |

## Window-client ABI

`WINDOW_*` syscalls are gated by `CAP_DISPLAY` (denials return `-EPERM`).

| Profile | Allowed syscalls |
|---------|------------------|
| `PROFILE_MINIMAL` | CLI set (no `SHM_*`, no `WINDOW_*`) |
| `PROFILE_GUI` | minimal + `SHM_*` + `WINDOW_*` |

GUI ELFs must be started with the kernel GUI spawn path (`proc_spawn_gui_path`
on Pegasus). For the reference app: start-menu **GUI Hello**, or Terminal
`spawn gui` / `spawn /bin/gui_hello`. See [GUI_SDK.md](GUI_SDK.md) for the
full `gui_hello` example.

`SYS_WINDOW_CREATE` takes a user pointer to `win_create_args_t` (title/geometry
in; window id, SHM mapping, pitch, surface size out). Pixels are `0xAARRGGBB`,
client-local. Events are delivered via `SYS_WINDOW_POLL_EVENT` into
`win_event_t` (SDK mirror: `pg_event_t` in `pegasus/gui_event.h`).

See [GUI_SDK.md](GUI_SDK.md) for the libgui API and event model.

## Shared memory

Named segments via `shm_create` / `shm_map` (`include/sys/shm.h`). Used by
libgui for client surfaces and by sandboxed renderer processes on Pegasus.

## Signals

POSIX signal numbers and delivery via `<signal.h>`. Blocking syscalls return
`-EINTR` when a deliverable signal is pending.

## Networking

Minimal TCP **client** only (`include/pegasus/socket.h`). Requires
`CAP_NET_CONNECT`.
