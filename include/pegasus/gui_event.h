#ifndef PEGASUS_GUI_EVENT_H
#define PEGASUS_GUI_EVENT_H

#include <stdint.h>

/* Userspace event types for SYS_WINDOW_POLL_EVENT / pg_window_poll.
 * Keep in sync with kernel/include/window_user.h (ABI mirror). */

#define PG_EV_CLOSE   1
#define PG_EV_KEY     2
#define PG_EV_MOUSE   3
#define PG_EV_RESIZE  4
#define PG_EV_WHEEL   5

#define PG_MOUSE_LEFT   1u
#define PG_MOUSE_RIGHT  2u
#define PG_MOUSE_DOWN   4u

/* Escape: PS/2 set-1 scancode 0x01 is translated to ASCII 27 in ps2_kbd.c.
 * The kernel also synthesizes PG_EV_CLOSE on Escape for userspace windows. */
#define PG_KEY_ESCAPE   27u

typedef struct {
    uint32_t type;
    int32_t  x, y;       /* mouse client coords; resize unused */
    int32_t  w, h;       /* resize: new client size */
    uint32_t key;        /* PG_EV_KEY: keycode (char or extended) */
    uint32_t buttons;    /* PG_EV_MOUSE: PG_MOUSE_* bits */
    int32_t  wheel;      /* PG_EV_WHEEL: delta */
    int32_t  pitch_px;   /* PG_EV_RESIZE: pixels per row */
    int32_t  shm_id;     /* PG_EV_RESIZE: surface segment */
    uint64_t shm_addr;   /* PG_EV_RESIZE: new user VA (0 if unchanged) */
} pg_event_t;

#endif
