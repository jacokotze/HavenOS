//the actual IO driver
#include "serial.h"
#include "io.h"
#include "vga.h"
#include "log.h"
#include "string.h"
// static char bad_serial = 'b';
unsigned char serial_open;

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),
         SERIAL_LINE_ENABLE_DLAB
        );
    outb(SERIAL_DATA_PORT(com),
         (divisor >> 8) & 0x00FF
        );
    outb(SERIAL_DATA_PORT(com),
         divisor & 0x00FF
        );
}

/** serial_configure_line:
 *  Configures the line of the given serial port. The port is set to have a
 *  data length of 8 bits, no parity bits, one stop bit and break control
 *  disabled.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_line(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
        * Content: | d | b | prty  | s | dl  |
        * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
        */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}


/** serial_configure_line:
 *  Configures the buffers of the given serial port. The port is set to have a
 *  @param com  The serial port to configure
 */
void serial_configure_buffers(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
        * Content: | d | b | prty  | s | dl  |
        * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
        */
    outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

/** serial_configure_modem:
 *  Configures the buffers of the given serial port. The port is set to have a
 *  @param com  The serial port to configure
 */
void serial_configure_modem(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
        * Content: | d | b | prty  | s | dl  |
        * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
        */
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
    serial_open = 1;
}

 /** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com)
{
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}


/** serial_write:
 *  write to the com port string c of length;
 *
 *  @param  com The COM port
 *  @param  c The string to write
 *  @param  length the length of string to write
 *  @return void
 */
void serial_write(unsigned short com, char* c, unsigned int length)
{
    if(serial_open) {
        for (unsigned int i = 0; i < length-1; i++)
        {
            while(!serial_is_transmit_fifo_empty(com));
            outb(SERIAL_DATA_PORT(com), c[i]);
        }
    } else {
        // fb_clear(0,0);
        // fb_print_lstr(c,strlen(c),2,1);
    }
}