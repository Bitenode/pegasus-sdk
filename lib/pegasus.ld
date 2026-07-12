ENTRY(_start)

SECTIONS
{
    . = 0x400000;

    .text : {
        *(.text .text.*)
    }

    /* Page-align so the read-only and read-write segments never share a page
       with executable code. The kernel enforces W^X (paging_map_allowed), and a
       page that is both writable and executable makes elf_load() fail. */
    . = ALIGN(0x1000);
    .rodata : {
        *(.rodata .rodata.*)
    }

    /* C++ exception-handling metadata. Read-only, so it lives with .rodata's
       segment (never on an executable page). Kept even though today's programs
       build -fno-exceptions: a future unwinder (P5) walks these via the
       eh_frame symbols below and __register_frame(). */
    .eh_frame_hdr : {
        __eh_frame_hdr_start = .;
        *(.eh_frame_hdr)
        *(.eh_frame_entry .eh_frame_entry.*)
        __eh_frame_hdr_end = .;
    }
    .eh_frame : {
        __eh_frame_start = .;
        KEEP(*(.eh_frame))
        *(.eh_frame.*)
        /* Zero-length CIE terminating the FDE list. GCC's crtend.o normally
           supplies this (__FRAME_END__); Pegasus links no crtend, so emit it
           here. __register_frame() (see userspace/libc/src/eh_register.c) walks
           frame descriptors from __eh_frame_start until it hits this 4-byte 0. */
        LONG(0)
        __eh_frame_end = .;
    }
    .gcc_except_table : {
        *(.gcc_except_table .gcc_except_table.*)
    }

    /* Page-align before the writable segment: executable .text must never share
       a page with writable data (W^X). */
    . = ALIGN(0x1000);

    /* Static constructor / destructor arrays. Empty for plain C programs
       (hello, coreutils), populated for C++ global objects (e.g.
       std::ios_base::Init). crt0 iterates __init_array_start..__init_array_end
       before main() and __fini_array in reverse afterwards. These pointers are
       resolved at link time for a static non-PIE, but are placed in the
       writable segment to match the GNU crt convention. */
    .init_array : {
        __init_array_start = .;
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array))
        KEEP(*(.ctors))
        __init_array_end = .;
    }
    .fini_array : {
        __fini_array_start = .;
        KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP(*(.fini_array))
        KEEP(*(.dtors))
        __fini_array_end = .;
    }

    .data : {
        *(.data .data.*)
    }

    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    }
}
