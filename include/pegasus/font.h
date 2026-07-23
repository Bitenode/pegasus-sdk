#ifndef PEGASUS_FONT_H
#define PEGASUS_FONT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Bundled 8×16 VGA-style bitmap font (ASCII 32–127). SDK copy. */

#define PG_FONT_WIDTH  8
#define PG_FONT_HEIGHT 16

/* Returns a pointer to 16 row bytes for glyph `c` (clamped to printable). */
const uint8_t *pg_font_glyph(char c);

void pg_font_draw_char(uint32_t *pixels, int pitch_px, int surf_w, int surf_h,
                       int x, int y, char c, uint32_t fg, uint32_t bg);
void pg_font_draw_string(uint32_t *pixels, int pitch_px, int surf_w, int surf_h,
                         int x, int y, const char *str, uint32_t fg, uint32_t bg);
/* Draw at 2× scale (16×32 effective cell size) for legibility on hi-res windows. */
void pg_font_draw_string_2x(uint32_t *pixels, int pitch_px, int surf_w, int surf_h,
                            int x, int y, const char *str, uint32_t fg, uint32_t bg);

#ifdef __cplusplus
}
#endif

#endif
