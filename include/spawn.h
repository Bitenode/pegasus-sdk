#ifndef PEGASUS_SPAWN_H
#define PEGASUS_SPAWN_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { int _reserved; } posix_spawn_file_actions_t;
typedef struct { int _reserved; } posix_spawnattr_t;

int posix_spawn(pid_t *pid, const char *path,
                const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], char *const envp[]);

#ifdef __cplusplus
}
#endif

#endif
