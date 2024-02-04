#ifndef COMMON_H
#define COMMON_H

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);
void memcpy(u8int *dest, const u8int *src, u32int len);
void memset(u8int *dest, u8int val, u32int len);
void* memmove(void* dest, const void* src, unsigned count);
int memcmp(const void* ptr1, const void* ptr2, unsigned count);
//halt and catch fire/ Inform the user something whent wrong; spin loop forever;
void PANIC(char str[]);

// #define PANIC(msg) panic(msg, __FILE__, __LINE__);
// #define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

// extern void panic(const char *message, const char *file, u32int line);
// extern void panic_assert(const char *file, u32int line, const char *desc);

#endif // COMMON_H