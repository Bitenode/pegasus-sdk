#ifndef PEGASUS_CAPABILITY_H
#define PEGASUS_CAPABILITY_H

/* Mirror kernel/include/capability.h — bitmask returned by getcaps(). */
#define CAP_NONE          0u
#define CAP_READ_HOME     (1u << 0)
#define CAP_WRITE_HOME    (1u << 1)
#define CAP_NET_CONNECT   (1u << 2)
#define CAP_DISPLAY       (1u << 3)
#define CAP_INPUT         (1u << 4)
#define CAP_FS_READ       (1u << 5)
#define CAP_FS_WRITE      (1u << 6)

#endif
