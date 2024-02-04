//
// isr.c -- High level interrupt service routines and interrupt request handlers.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include "isr.h"
#include "vga.h"
#include "string.h"
#include "io.h"
#include "log.h"

isr_t interrupt_handlers[256];

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
   // fb_clear(0,0);
   // fb_move_cursor_col_row(0,0);
   // fb_fprint_str("recieved interrupt: # {%d}",(int[]){regs.int_no}, 0,4);
   if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
   // fb_clear(11,11);
   // fb_move_cursor_col_row(0,0);
   // fb_fprint_str("FIRE IRQ Handler: # {%d}",(int[]){regs.int_no}, 0,4);
   // fb_clear(0,0);
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (regs.int_no >= 40)
   {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
   // fb_clear(0,0);
   // fb_move_cursor_col_row(0,0);
   // fb_fprint_str("recieved IRQ: # {%d} ?? {%d}",(int[]){regs.int_no, interrupt_handlers[regs.int_no]}, 0,4);
}

void register_interrupt_handler(unsigned char n, isr_t handler)
{
   // fb_clear(11,11);
   // fb_move_cursor_col_row(0,0);
   // fb_fprint_str("Register IRQ Handler: # {%d}",(int[]){n}, 0,4);
   // fb_print_cell(n, 1,0);
   // delay(1);
   // fb_clear(0,0);

  interrupt_handlers[n] = handler;
}
