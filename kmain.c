#include "io.h"
#include "vga.h"
#include "string.h"
#include "serial.h"
#include "timer.h"
#include "log.h"
#include "mem.h"
#include "interrupt_table.h"
#include "keyboard.h"
#include "paging.h"


#define STI \
__asm__\
( \
	"sti\n"\
	"nop\n"\
);

#define CLI \
__asm__\
( \
	"cli\n"\
	"nop\n"\
);

int kmain() {
    init_descriptor_tables();
    initialise_paging();
    
    serial_configure_baud_rate(SERIAL_COM1_BASE, 4);
    serial_configure_baud_rate(SERIAL_COM1_BASE, 4);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_configure_buffers(SERIAL_COM1_BASE);
    serial_configure_modem(SERIAL_COM1_BASE);
    // serial_ready();
    
    fb_clear(1,1);

    static char boot_message[] = "Serial Open!";
    print_debug(boot_message, sizeof(boot_message));
    
    // // the below will make the OS "run forever" instead of exiting on completion.
    // // while (1)
    // // {
    // //     ;
    // // }
    // init_timer(50); // Initialise timer to 50Hz
    asm volatile ("int $0x3");
    asm volatile("sti");
    init_timer(50);
    init_keyboard();
    fb_clear(0,15);
    fb_move_cursor(3);
    fb_print_cell('>',0,15);
    while(1) {
        ;
    }
}