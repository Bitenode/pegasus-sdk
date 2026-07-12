#ifndef PEGASUS_DIRENT_H
#define PEGASUS_DIRENT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DT_UNKNOWN 0
#define DT_FIFO    1
#define DT_CHR     2
#define DT_DIR     4
#define DT_BLK     6
#define DT_REG     8
#define DT_LNK     10
#define DT_SOCK    12
#define DT_WHT     14

/* Layout shared verbatim with the kernel (see SYS_GETDENTS). */
struct dirent {
    uint64_t d_ino;
    uint32_t d_type;
    char     d_name[256];
};

typedef struct {
    char path[256];
    int  index;
} DIR;

/*
 * Raw directory read: fills *out with the entry at the given zero-based index
 * within `path`. Returns 1 on success, 0 when index is past the last entry,
 * and -1 on error.
 */
int getdents(const char *path, int index, struct dirent *out);

DIR *opendir(const char *path);
struct dirent *readdir(DIR *d);
int closedir(DIR *d);

#ifdef __cplusplus
}
#endif

#endif
