#ifndef PEGASUS_WINDOW_H
#define PEGASUS_WINDOW_H

#include <stdint.h>
#include <pegasus/gui_event.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Pixel format is 0xAARRGGBB. Coordinates are client-local (origin at the
 * top-left of the client rect, below the title bar / inside the border). */

typedef struct pg_window pg_window_t;

pg_window_t *pg_window_create(const char *title, int x, int y, int w, int h);
uint32_t    *pg_window_buffer(pg_window_t *win, int *pitch_px);
int          pg_window_width(const pg_window_t *win);
int          pg_window_height(const pg_window_t *win);
void         pg_window_present(pg_window_t *win);
void         pg_window_present_rect(pg_window_t *win, int x, int y, int w, int h);
/* Returns 1 if an event was written, 0 if none, -1 on error (errno set). */
int          pg_window_poll(pg_window_t *win, pg_event_t *ev);
/* Like pg_window_poll but waits up to timeout_ms (0 = non-blocking). */
int          pg_window_poll_timeout(pg_window_t *win, pg_event_t *ev,
                                   uint32_t timeout_ms);
void         pg_window_destroy(pg_window_t *win);
/* Complete a pending surface grow and drain resize events (call each frame). */
void         pg_window_sync_surface(pg_window_t *win);

/* Helpers (operate on the current client buffer). */
void pg_fill_rect(pg_window_t *win, int x, int y, int w, int h, uint32_t color);
void pg_draw_string(pg_window_t *win, int x, int y, const char *str,
                    uint32_t fg, uint32_t bg);
void pg_draw_string_2x(pg_window_t *win, int x, int y, const char *str,
                       uint32_t fg, uint32_t bg);

#ifdef __cplusplus
}
#endif

#endif
