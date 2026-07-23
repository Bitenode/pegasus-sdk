#ifndef PEGASUS_SYS_SHM_H
#define PEGASUS_SYS_SHM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Named shared-memory segment: create-or-open by name, return segment id.
 * On failure returns -1 and sets errno. */
int shm_create(const char *name, size_t size);

/* Map segment `id` into the current process; returns the user VA, or
 * (void *)-1 on failure with errno set. */
void *shm_map(int id);

#ifdef __cplusplus
}
#endif

#endif
