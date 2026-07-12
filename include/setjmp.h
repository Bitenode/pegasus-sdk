#ifndef PEGASUS_SETJMP_H
#define PEGASUS_SETJMP_H

/*
 * Minimal x86-64 SysV setjmp/longjmp. The buffer stores the callee-saved
 * registers (rbx, rbp, r12-r15), the caller's stack pointer, and the return
 * address: 8 words total. This is all Lua's error handling (LUAI_THROW /
 * LUAI_TRY) requires.
 */
typedef long jmp_buf[8];

#ifdef __cplusplus
extern "C" {
#endif

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val) __attribute__((noreturn));

#ifdef __cplusplus
}
#endif

#endif
