#include "log.h"
#include "serial.h"
#include "string.h"

static char CHAR_NEW_LINE = '\n';
static char _debug[] = "[DEBUG] ";

void print_debug(char* str, unsigned int length) {
    serial_write(SERIAL_COM1_BASE, _debug, sizeof(_debug));
    serial_write(SERIAL_COM1_BASE, str, length+1);
    serial_write(SERIAL_COM1_BASE, &CHAR_NEW_LINE, sizeof(CHAR_NEW_LINE)+1);
}

void print_debug_str(char str[]) {
    serial_write(SERIAL_COM1_BASE, _debug, sizeof(_debug));
    serial_write(SERIAL_COM1_BASE, str, strlen(str)+1);
    serial_write(SERIAL_COM1_BASE, &CHAR_NEW_LINE, sizeof(CHAR_NEW_LINE)+1);
}

void print_debug_dec(int i) {
    serial_write(SERIAL_COM1_BASE, _debug, sizeof(_debug));
    char buffer[12];
    int_to_str(i, buffer);
    serial_write(SERIAL_COM1_BASE, buffer, strlen(buffer));
    serial_write(SERIAL_COM1_BASE, &CHAR_NEW_LINE, sizeof(CHAR_NEW_LINE)+1);
}

void delay(int i) {
    for (int j = 0; j < 999999*i; j++)
    {
        ;
    }
}