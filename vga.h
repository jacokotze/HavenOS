#ifndef INCLUDE_VGA_H
#define INCLUDE_VGA_H

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

#define FB_COLOR_BLACK 0
#define FB_COLOR_BLUE 1
#define FB_COLOR_GREEN 2
#define FB_COLOR_CYAN 3
#define FB_COLOR_RED 4
#define FB_COLOR_MAGENTA 5
#define FB_COLOR_BROWN 6
#define FB_COLOR_LIGHT_GREY 7
#define FB_COLOR_LIGHT_GRAY 7
#define FB_COLOR_DARK_GREY 8
#define FB_COLOR_DARK_GRAY 8
#define FB_COLOR_LIGHT_BLUE 9
#define FB_COLOR_LIGHT_GREEN 10
#define FB_COLOR_LIGHT_CYAN 11
#define FB_COLOR_LIGHT_RED 12
#define FB_COLOR_LIGHT_MAGENTA 13
#define FB_COLOR_LIGHT_BROWN 14
#define FB_COLOR_WHITE 15

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#define FB_NUM_COLUMNS 80
#define FB_NUM_ROWS 25


void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_write_str(unsigned int i, char* str, unsigned char fg, unsigned char bg) ;
void fb_write_lstr(unsigned int i, char* str, int length, unsigned char fg, unsigned char bg);

void fb_print_cell(char c, unsigned char fg, unsigned char bg);
void fb_print_str(char* str, unsigned char fg, unsigned char bg) ;
void fb_print_lstr(char* str, int length, unsigned char fg, unsigned char bg);

void fb_offset_cursor( int offset);
void fb_move_cursor(unsigned short pos);
void fb_move_cursor_col_row(unsigned short pos_x, unsigned short pos_y);


int fb_get_row( int i ) ;
int fb_get_current_row();
unsigned short fb_row_column_to_index(unsigned int column, unsigned int row);

void fb_clear_row(unsigned int r, unsigned char fg, unsigned char bg);
void fb_clear(unsigned char fg, unsigned char bg);
void fb_write_frect(char c, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char fg, unsigned char bg);
void fb_write_rect(char c, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char fg, unsigned char bg);
void fb_clear_rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color);
void fb_fprint_str(char str[], int args[], unsigned char fg, unsigned char bg);
void fb_scroll();
void fb_copy_row(int from, int to);
void fb_move_cursor_to_start_of_row();
void fb_shift_down(int from);
short get_cursor_pos();
#endif