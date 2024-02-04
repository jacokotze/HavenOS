#ifndef INCLIDE_PAGING_H
#define INCLIDE_PAGING_H
#define PAGE_SIZE 0x1000

extern unsigned int placement_address;
extern unsigned int endkernel;
extern void loadPageDirectory(unsigned int*);
extern void enablePaging();


// typedef struct page_directory_entry {
//    unsigned int present : 1;
//    unsigned int read_write : 1;
//    unsigned int user_supervisor : 1;
//    unsigned int write_through : 1;
//    unsigned int cache_disabled : 1;
//    unsigned int accessed : 1;
//    unsigned int reserved : 1;
//    unsigned int page_size : 1;
//    unsigned int global : 1;
//    unsigned int available : 3; // Available for custom use
//    unsigned int page_table_address : 20; // Physical address of the page table
// } page_directory_entry_t;

// typedef struct page_directory {
//     page_directory_entry_t entries[1024] __attribute__((aligned(4096)));
// } page_directory_t;

// page_directory_t page_directories[1024];


void switch_page_directory(unsigned int* pd);
void switch_page_directory_i(unsigned int i);

/**
  Sets up the environment, page directories etc and
  enables paging.
**/
void initialise_paging();

/**
  Handler for page faults.
**/
// void page_fault(registers_t regs);
#endif