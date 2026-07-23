# Pegasus GUI SDK

Windowed userspace apps paint into a **client shared-memory surface**. The
in-kernel window manager blits that buffer into the window's client rect each
frame. Input arrives through a per-window event ring polled by the app.

The compositor, chrome, desktop, and taskbar remain in the kernel. This document
covers the **libgui** client library shipped in this sysroot.

## Pixel format and coordinates

| Item | Rule |
|------|------|
| Pixel format | `0xAARRGGBB` (alpha in the high byte) |
| Origin | Client-local: `(0,0)` is the top-left of the client area (below the title bar, inside the border) |
| Pitch | `pitch_px` is pixels per row (may be ≥ width; use it as the row stride when writing pixels) |
| Present | `pg_window_present` marks pixels ready and returns the current client size |

On maximize/resize, call `pg_window_present` (or handle `PG_EV_RESIZE`) to learn
the new `width`/`height`, then **redraw the full client** and present again.
Escape is ASCII `27` (`PG_KEY_ESCAPE`); the kernel also synthesizes
`PG_EV_CLOSE` on Escape for userspace windows.

## Capabilities and seccomp

| Spawn path | Profile | Extra caps |
|------------|---------|------------|
| CLI (`execve` / `spawn` from `psh`) | `PROFILE_MINIMAL` | FS + net only — **no** `CAP_DISPLAY` |
| GUI (Terminal `spawn gui`, installed Start pin, `proc_spawn_gui_path`) | `PROFILE_GUI` | `CAP_DISPLAY` + `CAP_INPUT` |

`PROFILE_GUI` is `PROFILE_MINIMAL` plus `SYS_SHM_*` and `SYS_WINDOW_*`.
Without `CAP_DISPLAY`, every `WINDOW_*` call returns `-EPERM`.

See [ABI.md](ABI.md) for syscall numbers and spawn rules.

## Basic example — `gui_hello`

This is the in-tree reference app (`userspace/examples/gui_hello/gui_hello.c`,
also shipped as `examples/gui_hello.c` in this SDK). It creates a window, paints
centered text, redraws on resize/maximize, and quits on close / Escape / `q`.

### Build

```sh
export PATH="/path/to/pegasus-sdk/bin:$PATH"
pegasus-cc -o gui_hello.elf examples/gui_hello.c -lgui
```

Or from the Pegasus OS tree:

```sh
make gui_hello
# → userspace/examples/gui_hello/gui_hello.elf
```

### Run on Pegasus

Copy the ELF to the guest (e.g. seed `/bin/gui_hello` via `make disk`) and
launch with a **GUI spawn** path:

- Terminal: `spawn gui` / `spawn gui_hello` / `spawn /bin/gui_hello`
- Or package + install a `.pap` (Start pin / Desktop shortcut) — see
  [APP_PACKAGES.md](APP_PACKAGES.md)

A plain `psh` `spawn` without the GUI profile will get `-EPERM` on
`WINDOW_CREATE`.

### Ship as an installable package

Wrap the ELF in a `Something.pap/` folder with a `manifest` (`kind=gui`,
`binary=…`, optional `icon=`). Install from **Settings → Apps** or Terminal
`install`. Details, icon defaults, and Start → **Add to desktop** are in
[APP_PACKAGES.md](APP_PACKAGES.md).

### Source

```c
#include <pegasus/window.h>
#include <pegasus/font.h>
#include <string.h>

#define BG_COLOR   0xFF1E3A5F
#define FG_COLOR   0xFFFFFFFF
#define FG_MUTED   0xFFE8EEF5
#define SCALE      2

static int last_w, last_h;

static int text_width_2x(const char *str) {
    return (int)strlen(str) * PG_FONT_WIDTH * SCALE;
}

static void draw_frame(pg_window_t *win) {
    int w = pg_window_width(win);
    int h = pg_window_height(win);
    if (w <= 0 || h <= 0)
        return;

    pg_fill_rect(win, 0, 0, w, h, BG_COLOR);

    const char *title = "Hello from libgui!";
    const char *hint = "Resize or maximize - text stays centered.";
    const char *quit = "Close the window or press Escape.";

    int title_w = text_width_2x(title);
    int hint_w = (int)strlen(hint) * PG_FONT_WIDTH;
    int quit_w = (int)strlen(quit) * PG_FONT_WIDTH;
    int title_h = PG_FONT_HEIGHT * SCALE;
    int line_h = PG_FONT_HEIGHT;
    int block_h = title_h + 12 + line_h + 8 + line_h;
    int y0 = (h - block_h) / 2;
    if (y0 < 8)
        y0 = 8;

    int tx = (w - title_w) / 2;
    if (tx < 8) tx = 8;
    int hx = (w - hint_w) / 2;
    if (hx < 8) hx = 8;
    int qx = (w - quit_w) / 2;
    if (qx < 8) qx = 8;

    pg_draw_string_2x(win, tx, y0, title, FG_COLOR, BG_COLOR);
    pg_draw_string(win, hx, y0 + title_h + 12, hint, FG_MUTED, BG_COLOR);
    pg_draw_string(win, qx, y0 + title_h + 12 + line_h + 8, quit,
                   FG_MUTED, BG_COLOR);

    last_w = w;
    last_h = h;
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    pg_window_t *win = pg_window_create("GUI Hello", 120, 100, 520, 320);
    if (!win)
        return 1;

    draw_frame(win);
    pg_window_present(win);

    for (;;) {
        pg_event_t ev;
        int n = pg_window_poll_timeout(win, &ev, 16);
        if (n < 0)
            break;

        int need_redraw = 0;
        if (n == 1) {
            if (ev.type == PG_EV_CLOSE)
                break;
            if (ev.type == PG_EV_KEY &&
                (ev.key == PG_KEY_ESCAPE || ev.key == (uint32_t)'q' ||
                 ev.key == (uint32_t)'Q'))
                break;
            if (ev.type == PG_EV_RESIZE)
                need_redraw = 1;
        }

        /* PRESENT refreshes client width/height after maximize. */
        pg_window_present(win);
        if (need_redraw ||
            pg_window_width(win) != last_w ||
            pg_window_height(win) != last_h) {
            draw_frame(win);
            pg_window_present(win);
        }
    }

    pg_window_destroy(win);
    return 0;
}
```

## libgui API

Headers (this sysroot — not kernel includes):

- `pegasus/window.h` — window lifecycle + helpers
- `pegasus/gui_event.h` — `pg_event_t` / `PG_EV_*` / `PG_KEY_ESCAPE`
- `pegasus/blit.h`, `pegasus/font.h` — thin ARGB + 8×16 font helpers

| Call | Role |
|------|------|
| `pg_window_create` | `SYS_WINDOW_CREATE` — title/geom in; maps client SHM |
| `pg_window_buffer` | pointer to the ARGB surface + pitch |
| `pg_window_width` / `_height` | current logical client size |
| `pg_window_present` | present + refresh size from the kernel |
| `pg_window_poll` / `_timeout` | non-blocking / timed event read |
| `pg_window_destroy` | tear down window + SHM |
| `pg_fill_rect` / `pg_draw_string` / `_2x` | paint helpers |

Raw syscalls are also available via `pegasus/syscall.h` (`SYS_WINDOW_*` 42–47).

## Events

| Type | Meaning |
|------|---------|
| `PG_EV_CLOSE` | Title-bar close, Escape (kernel), or equivalent |
| `PG_EV_KEY` | Key press; Escape = `PG_KEY_ESCAPE` (27) |
| `PG_EV_MOUSE` | Click in client coords; `buttons` uses `PG_MOUSE_*` |
| `PG_EV_RESIZE` | Client size / surface changed — redraw |
| `PG_EV_WHEEL` | Wheel delta in `wheel` |

## Cross-compile notes

```sh
./bin/pegasus-cc myapp.c -o myapp.elf -lgui
```

Link order: your objects, then `-lgui`, then `libc.a` + `libcompiler_rt.a`
(`pegasus-cc` handles this automatically).

There is no userspace API yet to set a custom taskbar/window icon at runtime;
use the package `icon=` field (or the shell’s letter-tile default).
