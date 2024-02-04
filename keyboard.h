#ifndef INCLUDE_KEYBOARD_H
#define INCLUDE_KEYBOARD_H

struct struct_keyboard_event {
    unsigned char released;
    unsigned char scancode;
    unsigned char rscancode;
    unsigned char is_shift;
    unsigned char is_ctrl;
    unsigned char is_return;
    unsigned char is_tab;
    unsigned char is_pad;

    unsigned char is_pad_0 ;
    unsigned char is_pad_1 ;
    unsigned char is_pad_2 ;
    unsigned char is_pad_3 ;
    unsigned char is_pad_4 ;
    unsigned char is_pad_5 ;
    unsigned char is_pad_6 ;
    unsigned char is_pad_7 ;
    unsigned char is_pad_8 ;
    unsigned char is_pad_9 ;
    unsigned char is_pad_return ;
    unsigned char is_pad_backslash ;
    unsigned char is_pad_asterix ;
    unsigned char is_pad_period ;
    unsigned char is_pad_minus ;
    unsigned char is_pad_plus ;

    unsigned char is_capslock;
    unsigned char is_numeric;

    unsigned char is_minus;
    unsigned char is_equals;
    unsigned char is_open_brace;
    unsigned char is_close_brace;

    unsigned char is_arrow_left;
    unsigned char is_arrow_up;
    unsigned char is_arrow_down;
    unsigned char is_arrow_right;

    unsigned char is_comma;
    unsigned char is_period;
    unsigned char is_forwardslash;
    unsigned char is_colon;
    unsigned char is_quote;
    unsigned char is_pipe;
    unsigned char is_backtick;

    unsigned char is_command; //the WINDOWS key etc
    unsigned char is_command_b; // on some devices the [fn] key
    unsigned char is_page_up;
    unsigned char is_page_down;
    unsigned char is_home;
    unsigned char is_end;
    unsigned char is_insert;
    unsigned char is_delete;
    unsigned char is_backspace;
    unsigned char is_numlock;
    unsigned char is_escape;
    unsigned char is_space;
    unsigned char is_alt;
    // unsigned char is_calculate; // calculator button. cause why not!

    unsigned char sidedness; // 0 => left; 1 => right;

    unsigned char repeating; // 0 => no; 1 => yes;

    char key; 
};
typedef struct struct_keyboard_event keyboard_event;



keyboard_event read_scan_code(void);
char scancode_to_char(char code);
void init_keyboard();
char get_numeric(char scancode) ;
char get_is_pad(char scancode);
char get_scancode_sidedness(char scancode);
#endif