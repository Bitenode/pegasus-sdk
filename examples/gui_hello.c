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

        /* PRESENT refreshes client_w/h after maximize (same SHM, new size). */
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
