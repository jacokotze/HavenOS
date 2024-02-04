#include "common.h"
#include "kmemory.h"
#include "string.h"
#include "vga.h"
#include "isr.h"
#include "paging.h"


unsigned int kernel_page_directory[1024] __attribute__((aligned(4096)));
unsigned int kernel_page_table[1024] __attribute__((aligned(4096)));

unsigned int other_page_directory[1024] __attribute__((aligned(4096)));
unsigned int other_page_table[1024] __attribute__((aligned(4096)));

unsigned int directories[1024];
extern int other_location_start;

// Defined in kheap.c

// Macros used in the bitset algorithms.

void initialise_paging()
{
    fb_clear(0, 0);
    fb_move_cursor(0);
    fb_print_str("Setting up Paging...", 0, 15);
    fb_offset_cursor(FB_NUM_COLUMNS);
    fb_move_cursor_to_start_of_row();

    fb_print_str("Page Directory Setup...", 0, 15);
    fb_offset_cursor(FB_NUM_COLUMNS-1);
    fb_move_cursor_to_start_of_row();

    //set each entry to not present
    int i;
    for(i = 0; i < 1024; i++)
    {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        kernel_page_directory[i] = 0x00000002;
        other_page_directory[i] = 0x00000002;

        // fb_fprint_str("Create Directory Entry 0x{%h}", (int[]){i}, 0, 15);
        // fb_offset_cursor(FB_NUM_COLUMNS-1);
        // fb_move_cursor_to_start_of_row();
    }
    directories[0] = kernel_page_directory;
    directories[1] = other_page_directory;
    
    delay(9);
    fb_print_str("Page Table Setup...", 0, 15);
    fb_offset_cursor(FB_NUM_COLUMNS-1);
    fb_move_cursor_to_start_of_row();

    // holds the physical address where we want to start mapping these pages to.
    // in this case, we want to map these pages to the very beginning of memory.
    unsigned int j;
    
    //we will fill all 1024 entries in the table, mapping 4 megabytes
    for(j = 0; j < 1024; j++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        // fb_fprint_str("Map Physical Address 0x{%h}", (int[]){j}, 0, 15);
        // fb_offset_cursor(FB_NUM_COLUMNS-1);
        // fb_move_cursor_to_start_of_row();
        kernel_page_table[j] = (j * 0x1000) | 3; // attributes: supervisor level, read/write, present.
        other_page_table[j] = (j * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    
    }

    delay(9);
    //insert into
    fb_print_str("Insert Page Table -> Page Directory...", 0, 15);
    fb_offset_cursor(FB_NUM_COLUMNS-1);
    fb_move_cursor_to_start_of_row();

    

    // attributes: supervisor level, read/write, present
    kernel_page_directory[0] = ((unsigned int)kernel_page_table) | 3;
    other_page_directory[0] = ((unsigned int)other_page_table) | 3;
    delay(9);

    delay(3);
    switch_page_directory_i(0);
    delay(9);

    fb_print_str("Enabling Paging Mode...", 0, 15);
    fb_offset_cursor(FB_NUM_COLUMNS-1);
    fb_move_cursor_to_start_of_row();
    delay(3);
    enablePaging();
    delay(3);

    fb_print_str("Done?...", 0, 15);
    fb_offset_cursor(FB_NUM_COLUMNS-1);
    fb_move_cursor_to_start_of_row();
    // enable_paging();
}

void switch_page_directory_i(unsigned int i) {
    switch_page_directory(directories[i]);
}

void switch_page_directory(unsigned int* pd) {
    fb_fprint_str("Loading Page Directory {%h}", (int[]){pd}, 0, 15);
    fb_offset_cursor(FB_NUM_COLUMNS-1);
    fb_move_cursor_to_start_of_row();
    loadPageDirectory(pd);
}

void enable_paging() {
    u32int cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void page_fault(registers_t regs)
{
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    u32int faulting_address;
    asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

    // The error code gives us details of what happened.
    int present = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;         // Write operation?
    int us = regs.err_code & 0x4;         // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;   // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;        // Caused by an instruction fetch?

    char message[64] = "Page Fault ( ";

    if (present)
    {
        strcat(message, "present ", message);
    }
    if (rw)
    {
        strcat(message, "read-only ", message);
    }
    if (us)
    {
        strcat(message, "user-mode ", message);
    }
    if (reserved)
    {
        strcat(message, "reserved ", message);
    };
    strcat(message, ") at 0x", message);
    char hex[9];
    int_to_hex(faulting_address, hex);
    strcat(message, hex, message);
    PANIC(message);
}


// static pageframe_t kalloc_frame_int()
// {
//         u32int i = 0;
//         while(frame_map[i] != 0x00)
//         {
//                 i++;
//                 if(i == 1024)
//                 {
//                     PANIC("?");
//                 }
//         }
//         frame_map[i] = 0x01;
//         return(startframe + (i*0x1000));//return the address of the page frame based on the location declared free
//         //in the array
// }


// void kfree_frame(pageframe_t a)
// {
//                 a = a - startframe;//get the offset from the first frame
//                 if(a == 0)//in case it is the first frame we are freeing
//                 {
//                        uint32_t index = (uint32_t)a;
//                        frame_map[index] = FREE;    
//                 }
//                 else{ 
//                        a = a;//divide by 4kb to get the index to declare free
//                        uint32_t index = ((uint32_t)a)/0x1000;
//                        frame_map[index] = FREE;
//                     }
// }

// typedef struct k_page_frame {
//     int free : 1;
//     u32int physical_address; 
// } k_page_frame_t;

// //num frames : compute the number of frames required for the system memory using standard 4KB blocks
// //Thus in a 32MB system we have 32768 KB / 4 KB OR 8 frames
// #define num_frames 8

// k_page_frame_t k_page_frames[num_frames];

// void* kernel_get_free_frame() {
//     for (int i = 0; i < num_frames; i++)
//     {
//         if(k_page_frames[i].free) {
//             return &k_page_frames[i];
//         }
//     }
//     return 0; // failed to get page frame
// }

// void kernel_mark_frame_used(k_page_frame_t* f) {
//     f->free = 0x01;
// }

// void kernel_mark_frame_free(k_page_frame_t* f) {
//     f->free = 0x00;
// }

// void * find_and_claim_free_frame() {
//     k_page_frame_t* frame = (k_page_frame_t*)kernel_get_free_frame();
//     if (frame) {
//         kernel_mark_frame_used(frame);
//         return frame;
//     } else {
//         return 0;
//     }
// }

// void init_page_frames() {
//     for (int i = 0; i < num_frames; i++) {
//         k_page_frames[i].free = 0x00; // mark as not free
//         k_page_frames[i].physical_address = i * PAGE_SIZE; // initialize physical address
//     }
// }

