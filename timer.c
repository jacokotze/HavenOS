// timer.c -- Initialises the PIT, and handles clock updates.
// Written for JamesM's kernel development tutorials.

#include "timer.h"
#include "isr.h"
#include "io.h"
#include "vga.h"
#include "log.h"
unsigned int tick = 0;

static void timer_callback(registers_t regs)
{
    tick++;
    // fb_move_cursor_col_row(0,FB_NUM_ROWS-1);
    // fb_clear_row(FB_NUM_ROWS-1, 0,1);
    // fb_fprint_str("Timer @ {%d} -> {%d}", (int[]){tick, regs.int_no}, 0,1);
    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (tick % 50 == 0)
    {
        // fb_write_str(/"One Second Has Passed", 10,1);
    }
}

void init_timer(unsigned int frequency)
{
    fb_fprint_str("Initializing Timer {%d} -> {%\n}", (int[]){frequency}, 1,1);

    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ0, &timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    unsigned int divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    unsigned char l = (unsigned char)(divisor & 0xFF);
    unsigned char h = (unsigned char)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
}