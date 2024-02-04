#include "isr.h"
#include "string.h"
#include "io.h"
#include "interrupt_table.h"
#include "mem.h"
#include "vga.h"
#include "log.h"
#include "common.h"

// Extern the ISR handler array so we can nullify them on startup.
extern isr_t interrupt_handlers[];

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;
static void idt_set_gate(unsigned char, unsigned int, unsigned short, unsigned char);
extern void load_idt(unsigned int);

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define IDT_SET_GATE(index) \
    idt_set_gate(index, (unsigned int)isr##index, 0x08, 0x8E);

void init_descriptor_tables()
{
    init_gdt();

    init_idt();
    // Nullify all the interrupt handlers.
    memset(&interrupt_handlers, 0, sizeof(isr_t) * 256);
}

void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (unsigned int)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    outb(PIC1_COMMAND, 0x11); // initialise pic 1
    outb(PIC2_COMMAND, 0x11); // initialise pic 2

    outb(PIC1_DATA, PIC1_COMMAND); // end of interrupt
    outb(PIC2_DATA, 0x28);

    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);

    idt_set_gate(0, (u32int)isr0, 0x08, 0x8E);
    idt_set_gate(1, (u32int)isr1, 0x08, 0x8E);
    idt_set_gate(2, (u32int)isr2, 0x08, 0x8E);
    idt_set_gate(3, (u32int)isr3, 0x08, 0x8E);
    idt_set_gate(4, (u32int)isr4, 0x08, 0x8E);
    idt_set_gate(5, (u32int)isr5, 0x08, 0x8E);
    idt_set_gate(6, (u32int)isr6, 0x08, 0x8E);
    idt_set_gate(7, (u32int)isr7, 0x08, 0x8E);
    idt_set_gate(8, (u32int)isr8, 0x08, 0x8E);
    idt_set_gate(9, (u32int)isr9, 0x08, 0x8E);
    idt_set_gate(10, (u32int)isr10, 0x08, 0x8E);
    idt_set_gate(11, (u32int)isr11, 0x08, 0x8E);
    idt_set_gate(12, (u32int)isr12, 0x08, 0x8E);
    idt_set_gate(13, (u32int)isr13, 0x08, 0x8E);
    idt_set_gate(14, (u32int)isr14, 0x08, 0x8E);
    idt_set_gate(15, (u32int)isr15, 0x08, 0x8E);
    idt_set_gate(16, (u32int)isr16, 0x08, 0x8E);
    idt_set_gate(17, (u32int)isr17, 0x08, 0x8E);
    idt_set_gate(18, (u32int)isr18, 0x08, 0x8E);
    idt_set_gate(19, (u32int)isr19, 0x08, 0x8E);
    idt_set_gate(20, (u32int)isr20, 0x08, 0x8E);
    idt_set_gate(21, (u32int)isr21, 0x08, 0x8E);
    idt_set_gate(22, (u32int)isr22, 0x08, 0x8E);
    idt_set_gate(23, (u32int)isr23, 0x08, 0x8E);
    idt_set_gate(24, (u32int)isr24, 0x08, 0x8E);
    idt_set_gate(25, (u32int)isr25, 0x08, 0x8E);
    idt_set_gate(26, (u32int)isr26, 0x08, 0x8E);
    idt_set_gate(27, (u32int)isr27, 0x08, 0x8E);
    idt_set_gate(28, (u32int)isr28, 0x08, 0x8E);
    idt_set_gate(29, (u32int)isr29, 0x08, 0x8E);
    idt_set_gate(30, (u32int)isr30, 0x08, 0x8E);
    idt_set_gate(31, (u32int)isr31, 0x08, 0x8E);

    idt_set_gate(32, (u32int)irq0, 0x08, 0x8E);
    idt_set_gate(33, (u32int)irq1, 0x08, 0x8E);
    idt_set_gate(34, (u32int)irq2, 0x08, 0x8E);
    idt_set_gate(35, (u32int)irq3, 0x08, 0x8E);
    idt_set_gate(36, (u32int)irq4, 0x08, 0x8E);
    idt_set_gate(37, (u32int)irq5, 0x08, 0x8E);
    idt_set_gate(38, (u32int)irq6, 0x08, 0x8E);
    idt_set_gate(39, (u32int)irq7, 0x08, 0x8E);
    idt_set_gate(40, (u32int)irq8, 0x08, 0x8E);
    idt_set_gate(41, (u32int)irq9, 0x08, 0x8E);
    idt_set_gate(42, (u32int)irq10, 0x08, 0x8E);
    idt_set_gate(43, (u32int)irq11, 0x08, 0x8E);
    idt_set_gate(44, (u32int)irq12, 0x08, 0x8E);
    idt_set_gate(45, (u32int)irq13, 0x08, 0x8E);
    idt_set_gate(46, (u32int)irq14, 0x08, 0x8E);
    idt_set_gate(47, (u32int)irq15, 0x08, 0x8E);
    load_idt((unsigned int)&idt_ptr);
}

// called by asm
void load_idt_c()
{
    fb_fprint_str("Loading IDT Pointer#{%d}{%\n}", (int[]){(unsigned int)&idt_ptr}, 0, 4);
    // hand over to the ASM the pointer
    load_idt((unsigned int)&idt_ptr);
    return;
}

static void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel = sel;   // 0x08?
    idt_entries[num].always0 = 0; // RESERVED
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags = flags /* | 0x60 */; // Storage segment + DPL + Present
    fb_fprint_str("Loading IDT #{%d}{%\n}", (int[]){num}, 0, 4);
}
