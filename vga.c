#include "vga.h"
#include "io.h"
#include "string.h"
#include "log.h"

short cursor_pos = 0;
static char *fb = (char *)0x000B8000; // the VGA frame buffer pointer

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    cursor_pos = pos;

    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((cursor_pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, cursor_pos & 0x00FF);
}

short get_cursor_pos() {
    return cursor_pos;
}

/** fb_move_cursor_col_row:
 *  Moves the cursor of the framebuffer to the given position using column, row specification
 *
 *  @param pos_x The new position of the cursor column
 *  @param pos_y The new position of the cursor row
 *
 */
void fb_move_cursor_col_row(unsigned short pos_x, unsigned short pos_y)
{
    cursor_pos = fb_row_column_to_index(pos_x, pos_y);

    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((cursor_pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, cursor_pos & 0x00FF);
}

#define POSITIVE 1
#define NEGATIVE -1


/** fb_offset_cursor:
 *  Shifts the cursor of the framebuffer by some offset
 *
 *  @param pos The new position of the cursor; Sign indicates direction.
 */
void fb_offset_cursor(signed int offset)
{
    if (offset > (0)) {
        while(offset > 0) {
            cursor_pos = cursor_pos + 1;
            offset--;
        }
    } else {
        while(offset < 0) {
            cursor_pos = cursor_pos - 1;
            offset++;
        }
    }
    while(cursor_pos > FB_NUM_COLUMNS * FB_NUM_ROWS) {
        cursor_pos -= FB_NUM_COLUMNS;
        fb_scroll();
    }    
    while(cursor_pos < 0) {
        cursor_pos += FB_NUM_COLUMNS;
        fb_shift_down(FB_NUM_ROWS);
    } 

    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((cursor_pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, cursor_pos & 0x00FF);
}

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    i *= 2;
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

/** fb_print_cell:
 *  Prints a character with the given foreground and background to the current cursor position. Moves the cursor;
 *  in the framebuffer.
 *
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_print_cell(char c, unsigned char fg, unsigned char bg)
{
    fb_write_cell(cursor_pos, c, fg, bg);
    fb_offset_cursor(1);
}

/** fb_write_str:
 *  Writes a string with the given foreground and background from position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param str  The char* string pointer
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_str(unsigned int i, char *str, unsigned char fg, unsigned char bg)
{
    // Write each character and color to the framebuffer
    while (*str != '\0')
    {
        fb_write_cell(i, *str, fg, bg);
        i++;   // Move to the next character position in the framebuffer
        str++; // Move to the next character in the string
    }
}

/** fb_print_str:
 *  Prints a string with the given foreground and background at the cursor.
 *  Offsets the cursor
 *  in the framebuffer.
 *
 *  @param str  The char* string pointer
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_print_str(char str[], unsigned char fg, unsigned char bg)
{
    fb_write_lstr(cursor_pos, str, strlen(str), fg, bg);
    fb_offset_cursor(strlen(str));
}

/** fb_write_lstr:
 *  Writes a string with the given foreground and background from position i
 *  with a fixed length
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param str  The char* string pointer
 *  @param length The length to limit the string to
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_lstr(unsigned int i, char *str, int length, unsigned char fg, unsigned char bg)
{
    int j = 0;

    // Write each character and color to the framebuffer
    while (j < length)
    {
        fb_write_cell(i, *str, fg, bg);
        i++;   // Move to the next character position in the framebuffer
        str++; // Move to the next character in the string
        j++;
    }
}

/** fb_print_lstr:
 *  Prints a string with the given foreground and background starting at cursor;
 *  with a fixed length
 *
 *  @param str  The char* string pointer
 *  @param length The length to limit the string to
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_print_lstr(char *str, int length, unsigned char fg, unsigned char bg)
{
    //-1 for the null char in any string?; seems wrong and perhaps its my lengths that are wrong?
    fb_write_lstr(cursor_pos, str, length, fg, bg);
    fb_offset_cursor(length);
}

int fb_get_current_row()
{
    return fb_get_row(cursor_pos);
}

/** fb_get_row:
 * Get the row of the ith position in frame buffer according to #FB_NUM_COLUMNS and #FB_NUM_ROWS
 *  @param i the position to move to fetch the row of;
 */
int fb_get_row(int i)
{
    return (i / FB_NUM_COLUMNS);
}

/** fb_row_column_to_index:
 * Get the ith item of the 1D framebuffer from its 2D position;
 *  @param column the column of the position
 *  @param row the row of the position
 */
unsigned short fb_row_column_to_index(unsigned int column, unsigned int row)
{
    return row * FB_NUM_COLUMNS + column;
}

/** fb_clear_row:
 * clear the row'th row
 *  @param row the row to clear
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_clear_row(unsigned int r, unsigned char fg, unsigned char bg)
{
    int i = FB_NUM_COLUMNS * r;
    for (int col = 0; col < FB_NUM_COLUMNS; col++)
    {
        fb_write_cell(i++, ' ', fg, bg);
    }
}

/** fb_clear;
 * clear the fb
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_clear(unsigned char fg, unsigned char bg)
{
    for (int row = 0; row < FB_NUM_ROWS; row++)
    {
        fb_clear_row(row, fg, bg);
    }
}

/** fb_write_frect;
 * fill a rectangular area in the fb specified by x,y,width,height with character c
 *  @param c The character to fill the area with
 *  @param x the x position
 *  @param y the y position
 *  @param width the width of the rectangle
 *  @param height the height of the rectangle
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_frect(char c, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char fg, unsigned char bg)
{
    int cell;
    for (unsigned int cx = 0; cx < width; cx++)
    {
        for (unsigned int cy = 0; cy < height; cy++)
        {
            cell = (y + cy) * FB_NUM_COLUMNS + (cx + x);
            fb_write_cell(cell, c, fg, bg);
        }
    }
}

/** fb_write_rect;
 * draw a rectangular area perimeter in the fb specified by x,y,width,height with character c
 *  @param c The character to fill the area with
 *  @param x the x position
 *  @param y the y position
 *  @param width the width of the rectangle
 *  @param height the height of the rectangle
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_rect(char c, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char fg, unsigned char bg)
{
    int cell;
    for (unsigned int cx = 0; cx < width; cx++)
    {
        for (unsigned int cy = 0; cy < height; cy++)
        {
            if (cy == 0 || cy == height - 1 || cx == 0 || cx == width - 1)
            {
                cell = (y + cy) * FB_NUM_COLUMNS + (cx + x);
                fb_write_cell(cell, c, fg, bg);
            }
        }
    }
}

/** fb_write_frect;
 * fill a rectangular area in the fb specified by x,y,width,height with a empty character
 *  @param x the x position
 *  @param y the y position
 *  @param width the width of the rectangle
 *  @param height the height of the rectangle
 *  @param color The color to fill with
 */
void fb_clear_rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color)
{
    fb_write_frect(' ', x, y, width, height, color, color);
}

char fb_get_char(unsigned short i)
{
    return fb[i*2];
}

char fb_get_attributes(unsigned short i)
{
    return fb[i*2 + 1];
}

char fb_get_char_rc(unsigned int x, unsigned int y)
{
    return fb[fb_row_column_to_index(x,y)];
}

char fb_get_attributes_rc(unsigned int x, unsigned int y)
{
    return fb[fb_row_column_to_index(x,y)+1];
}

void fb_copy_row(int from, int to)
{
    if(from < 0 || from > FB_NUM_ROWS || to < 0 || to > FB_NUM_ROWS) {
        return;
    }
    unsigned short row_in_start = fb_row_column_to_index(0,from);
    unsigned short row_out_start = fb_row_column_to_index(0,to);
    
    for (unsigned short i = 0; i <= FB_NUM_COLUMNS; i++)
    {
        fb[(row_out_start++)*2] = fb[(row_in_start++)*2];
        fb[(row_out_start)*2 +1 ] = fb[(row_in_start)*2 +1];
    }
    
}

void fb_shift_down(int from) {
    //shift the contents of buffer down
    for (int row = FB_NUM_ROWS; row >= from; row--)
    {
        fb_copy_row(row, row+1);
    }
}

void fb_move_cursor_to_start_of_row() {
    fb_move_cursor_col_row(0, fb_get_current_row());
}

void fb_scroll()
{
    unsigned int row = 1; 
    for (;row < FB_NUM_ROWS; row++)
    {
        fb_copy_row(row,row-1);
    }
    unsigned color = fb[(FB_NUM_COLUMNS*FB_NUM_ROWS)*2-1];
    fb_clear_row(FB_NUM_ROWS-1, color,color);
    
}

/// @brief  EXAMPLE:
/// int number = 1024;
///    char buffer[12];  // Assuming a maximum of 12 digits for the integer
///    int_to_str(number, buffer);
///    // Now, 'buffer' contains the string representation of 'number'
/// @param num
/// @param buffer
void fb_fprint_str(char str[], int args[], unsigned char fg, unsigned char bg)
{
    int cursor = 0;
    int opener;
    int hintIndex = 0;

    while (str[cursor] != '\0')
    {
        char c = str[cursor];
        if (c == '{' && str[cursor + 1] == '%')
        {
            opener = cursor;
            while (str[cursor] != '}')
            {
                cursor++;
            }

            // Extract the type hint
            char typeHint[cursor - opener + 1];
            strslice(str, opener, cursor, typeHint);
            typeHint[cursor - opener + 1] = '\0';

            if (strcmp(typeHint, "{%d}") == 0)
            {
                // Handle integer type hint
                char intAsString[32];
                int_to_str(args[hintIndex], intAsString);
                fb_print_lstr(intAsString, strlen(intAsString), fg, bg);
            }
            else if (strcmp(typeHint, "{%\n}") == 0)
            {
                // Handle ne wline hint
                // fb_move_cursor_col_row(0,fb_get_current_row()+1);
                fb_offset_cursor(FB_NUM_COLUMNS);
                fb_move_cursor_to_start_of_row();
            }
            else if (strcmp(typeHint, "{%h}") == 0)
            {
                // Handle integer type hint
                char intAsString[9];
                int_to_hex(args[hintIndex], intAsString);
                fb_print_lstr(intAsString, strlen(intAsString), fg, bg);
            }
            else
            {
                fb_print_cell('?', fg, bg);
                fb_print_lstr(typeHint, strlen(typeHint), fg, bg);
            }

            hintIndex++;
        }
        else
        {
            fb_print_cell(c, fg, bg);
        }
        cursor++;
    }
}

void fb_inform_isr_done()
{
    fb_clear(1,1);
    fb_move_cursor(0);
    fb_print_str("ISR loading has completed.", 0,1);
    delay(9);
    return;
}

void fb_inform_irq_done()
{
    fb_move_cursor_col_row(0,1);
    fb_print_str("IRQ loading has completed.", 0,1);
    delay(9);
    return;
}