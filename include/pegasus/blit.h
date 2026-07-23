#ifndef PEGASUS_BLIT_H
#define PEGASUS_BLIT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Thin ARGB helpers over a pitched pixel buffer (SDK copy; not kernel blit.h). */

void pg_blit_fill_rect(uint32_t *pixels, int pitch_px, int surf_w, int surf_h,
                       int x, int y, int w, int h, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif
