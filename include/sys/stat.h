#ifndef PEGASUS_SYS_STAT_H
#define PEGASUS_SYS_STAT_H

#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Fixed 48-byte layout shared verbatim with the kernel (see SYS_STAT in
 * kernel/syscall/syscall.c). All 8-byte members come first to avoid padding
 * ambiguity between toolchains.
 */
struct stat {
    uint64_t st_dev;
    uint64_t st_ino;
    uint64_t st_nlink;
    off_t    st_size;
    time_t   st_mtime;
    mode_t   st_mode;
    mode_t   st_uid;
};

#define S_IFMT  0170000
#define S_IFREG 0100000
#define S_IFDIR 0040000
#define S_IFLNK 0120000
#define S_IFCHR 0020000
#define S_IFBLK 0060000
#define S_IFIFO 0010000
#define S_IFSOCK 0140000

#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)  (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)

/* POSIX permission bits (libstdc++ <filesystem> maps std::filesystem::perms
 * onto these). Pegasus does not enforce them, but the constants must exist. */
#define S_ISUID 04000
#define S_ISGID 02000
#define S_ISVTX 01000
#define S_IRWXU 0700
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRWXG 0070
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IRWXO 0007
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001

int stat(const char *path, struct stat *st);
int fstat(int fd, struct stat *st);

/* lstat: Pegasus has no symlinks, so it is identical to stat(). */
int lstat(const char *path, struct stat *st);

/* Metadata mutators Pegasus does not implement. Present so cross-built
 * libstdc++ <filesystem> links; they fail with errno=ENOSYS. */
int chmod(const char *path, mode_t mode);
int fchmod(int fd, mode_t mode);
int mkfifo(const char *path, mode_t mode);

#ifdef __cplusplus
}
#endif

#endif
