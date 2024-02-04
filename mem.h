#ifndef INCLUDE_MEM_H
#define INCLUDE_MEM_H

// Define the GDT entries
struct gdt_entry_struct {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

// Define the GDT pointer structure
struct gdt_ptr_struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

// These extern directives let us access the addresses of our ASM ISR handlers.

// Initialisation function is publicly accessible.
void load_gdt_c();



#endif