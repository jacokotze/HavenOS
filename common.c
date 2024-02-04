#include "common.h"
#include "vga.h"
#include "string.h"

u16int inw(u16int port)
{
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Copy len bytes from src to dest.
void memcpy(u8int *dest, const u8int *src, u32int len)
{
    const u8int *sp = (const u8int *)src;
    u8int *dp = (u8int *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

// Write len copies of val into dest.
void memset(u8int *dest, u8int val, u32int len)
{
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}


// Implementation of memmove
void* memmove(void* dest, const void* src, unsigned count) {
    unsigned char* d = dest;
    const unsigned char* s = src;

    if (d < s || d >= s + count) {
        while (count-- > 0) {
            *d++ = *s++;
        }
    } else {
        d = d + count - 1;
        s = s + count - 1;
        while (count-- > 0) {
            *d-- = *s--;
        }
    }

    return dest;
}

// Implementation of memcmp
int memcmp(const void* ptr1, const void* ptr2, unsigned count) {
    const unsigned char* p1 = ptr1;
    const unsigned char* p2 = ptr2;
    while (count-- > 0) {
        if (*p1 != *p2) {
            return (*p1 < *p2) ? -1 : 1;
        }
        p1++;
        p2++;
    }
    return 0;
}


void PANIC(char *str) {
    fb_clear(0,0);
    fb_clear(FB_COLOR_RED,FB_COLOR_RED);
    int col = (FB_NUM_COLUMNS/2) - ((strlen(str)+1)/2);
    if(!col) {
        col = 0;
    }
    fb_move_cursor_col_row(col,FB_NUM_ROWS/2);
    fb_print_str(str, FB_COLOR_RED, FB_COLOR_BLACK);
    while(1){;};
}


// extern void panic(const char *message, const char *file, u32int line)
// {
//     // We encountered a massive problem and have to stop.
//     asm volatile("cli"); // Disable interrupts.

//     monitor_write("PANIC(");
//     monitor_write(message);
//     monitor_write(") at ");
//     monitor_write(file);
//     monitor_write(":");
//     monitor_write_dec(line);
//     monitor_write("\n");
//     // Halt by going into an infinite loop.
//     for(;;);
// }

// extern void panic_assert(const char *file, u32int line, const char *desc)
// {
//     // An assertion failed, and we have to panic.
//     asm volatile("cli"); // Disable interrupts.

//     monitor_write("ASSERTION-FAILED(");
//     monitor_write(desc);
//     monitor_write(") at ");
//     monitor_write(file);
//     monitor_write(":");
//     monitor_write_dec(line);
//     monitor_write("\n");
//     // Halt by going into an infinite loop.
//     for(;;);
// }
