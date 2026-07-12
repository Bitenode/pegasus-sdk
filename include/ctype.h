#ifndef PEGASUS_CTYPE_H
#define PEGASUS_CTYPE_H

/*
 * Newlib-style character-class bitmask macros. GNU libstdc++ built --with-newlib
 * selects config/os/generic/ctype_base.h, whose std::ctype_base::mask constants
 * are defined in terms of these (_U/_L/_N/_S/_P/_C/_X/_B). The mask type there is
 * `char`, so _B (0x80) intentionally occupies the sign bit. These are only used
 * to compose the mask enum; the classification is done by the is*() functions.
 */
#define _U 0x01  /* upper */
#define _L 0x02  /* lower */
#define _N 0x04  /* digit */
#define _S 0x08  /* space */
#define _P 0x10  /* punct */
#define _C 0x20  /* control */
#define _X 0x40  /* hex digit */
#define _B 0x80  /* printable space (blank) */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Newlib-compatible classification table. GNU libstdc++ (config/os/newlib)
 * implements std::ctype<char>::classic_table() as `_ctype_ + 1`, so index 0 is
 * the EOF slot and indices 1..256 classify bytes 0..255 using the _U/_L/... bits
 * above. Defined in libc/src/ctype.c.
 */
extern const char _ctype_[];

int isalnum(int c);
int isalpha(int c);
int isblank(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int isascii(int c);
int toascii(int c);
int tolower(int c);
int toupper(int c);

#ifdef __cplusplus
}
#endif

#endif
