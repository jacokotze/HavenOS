#include "keyboard.h"
#include "timer.h"
#include "isr.h"
#include "io.h"
#include "vga.h"
#include "log.h"
#include "string.h"
#include "common.h"
#include "paging.h"

#define KBD_DATA_PORT   0x60
#define KBD_CONTROL_PORT   0x64

char KB_SHIFT = 0;
char KB_CTRL = 0;
char KB_ALT = 0;
char KB_NUMLOCK = 0;

const int KBD_RELEASE = 0x80;

/** read_scan_code:
 *  Reads a scan code from the keyboard
 *
 *  @return The scan code (NOT an ASCII character!)
 */
keyboard_event read_scan_code(void)
{
    char scancode = inb(KBD_DATA_PORT);
    keyboard_event k = {
        .released = (scancode & KBD_RELEASE),
        .scancode = scancode,
        .rscancode = 0,
        .is_shift = 0,
        .is_ctrl = scancode == 0x1D || scancode == 0x9D, //no sidedness
        .is_alt = scancode == 0x38 || scancode == 0xB8, //no sidedness
        .is_return = scancode == 0x1C || scancode == (0x1C + KBD_RELEASE),
        .is_tab = 0,
        .is_pad = 0,//get_is_pad(scancode), disabled until i figure out numlock ->
        .is_pad_0 = scancode == 0x52 || scancode == (0x52 + KBD_RELEASE),
        .is_pad_1 = scancode == 0x4F || scancode == (0x4F + KBD_RELEASE),
        .is_pad_2 = scancode == 0x50 || scancode == (0x50 + KBD_RELEASE),
        .is_pad_3 = scancode == 0x51 || scancode == (0x51 + KBD_RELEASE),
        .is_pad_4 = scancode == 0x4B || scancode == (0x4B + KBD_RELEASE),
        .is_pad_5 = scancode == 0x4C || scancode == (0x4C + KBD_RELEASE),
        .is_pad_6 = scancode == 0x4D || scancode == (0x4D + KBD_RELEASE),
        .is_pad_7 = (scancode == 0x47 || scancode == (0x47 + KBD_RELEASE)),
        .is_pad_8 = scancode == 0x48 || scancode == (0x48 + KBD_RELEASE),
        .is_pad_9 = scancode == 0x49 || scancode == (0x49 + KBD_RELEASE),
        .is_pad_asterix = scancode == 0x37 || scancode == (0x37 + KBD_RELEASE),
        .is_pad_period = scancode == 0x53 || scancode == (0x53 + KBD_RELEASE),
        .is_pad_minus = scancode == 0x4A || scancode == (0x4A + KBD_RELEASE),
        .is_pad_plus = scancode == 0x4E || scancode == (0x4E + KBD_RELEASE),

        .is_minus = scancode == 0x0C || scancode == (0x0C + KBD_RELEASE),
        .is_equals = scancode == 0x0D || scancode == (0x0D + KBD_RELEASE),
        .is_open_brace = scancode == 0x1A || scancode == (0x1A + KBD_RELEASE),
        .is_close_brace = scancode == 0x1B || scancode == (0x1B + KBD_RELEASE),
        
        .is_comma = scancode == 0x33 || scancode == (0x33 + KBD_RELEASE),
        .is_period = scancode == 0x34 || scancode == (0x34 + KBD_RELEASE),
        .is_forwardslash = scancode == 0x35 || scancode == (0x35 + KBD_RELEASE),
        .is_colon = scancode == 0x27 || scancode == (0x27 + KBD_RELEASE),
        .is_quote = scancode == 0x28 || scancode == (0x28 + KBD_RELEASE),
        .is_pipe = scancode == 0x2B || scancode == (0x2B + KBD_RELEASE),
        .is_backtick = scancode == 0x29 || scancode == (0x29 + KBD_RELEASE),

        .is_arrow_left = scancode == 0x4B || scancode == (0x4B + KBD_RELEASE),
        .is_arrow_up = scancode == 0x48 || scancode == (0x48 + KBD_RELEASE),
        .is_arrow_down = scancode == 0x50 || scancode == (0x50 + KBD_RELEASE),
        .is_arrow_right = scancode == 0x4D || scancode == (0x4D + KBD_RELEASE),


        .is_capslock = scancode == 0x3A || scancode == (0x3A + KBD_RELEASE),
        .is_numeric = 0, //1-0 and (num 1-0 with num lock on;)

        .is_command  = scancode == 0, //the WINDOWS key etc
        .is_command_b  = scancode == 0, // on some devices the [fn] key; apparently this doesnt generate a code, just modifies!?
        .is_page_up = scancode == 0x49 || scancode == (0x49 + KBD_RELEASE),
        .is_page_down = scancode == 0x51 || scancode == (0x51 + KBD_RELEASE),
        .is_home  = scancode == 0x47 || scancode == (0x47 + KBD_RELEASE),
        .is_end  = scancode == 0x4F || scancode == (0x4F + KBD_RELEASE),
        .is_insert  = scancode == 0x52 || scancode == (0x52 + KBD_RELEASE),
        .is_delete  = scancode == 0x53 || scancode == (0x53 + KBD_RELEASE),
        .is_backspace  = scancode == 0x0E || scancode == (0x0E + KBD_RELEASE),
        .is_numlock  = scancode == 0x45 || scancode == (0x45 + KBD_RELEASE),
        .is_escape  = scancode == 0x01 || scancode == (0x01 + KBD_RELEASE),
        .is_space  = scancode == 0x39 || scancode == (0x39 + KBD_RELEASE),
        // .is_calculate  = scancode == , // calculator button. cause why not!

        .sidedness = 0, // 0 => left , 1 => right ,

        .repeating = 0, // 0 => no , 1 => yes ,
    };
    if(scancode & 0x80) {
        k.rscancode = scancode-0x80;
    } else {
        k.rscancode = scancode;
    }
    if(k.scancode == 0x0F || k.scancode == 0x80) {
        k.is_tab = 1;
        k.rscancode = 0x0F;
    }
    k.is_numeric = get_numeric(k.rscancode) > -1;
    k.sidedness = get_scancode_sidedness(k.rscancode);
    k.is_shift = k.rscancode  == 0x2A || k.rscancode == 0x36;
    return k;
}

char compute_key(keyboard_event *k) {
    if(k->is_shift) {
        if(k->scancode == 0xAA || k->scancode == 0xB6) {
            KB_SHIFT = 0;
            fb_write_cell(1,'s',0,15);
        } else {
            fb_write_cell(1,'S',0,15);
            KB_SHIFT = 1;
        }
        return '\0';
    }

    if(k->is_ctrl) {
        KB_CTRL= k->released == 0;
        return '\0';
    }

    if(k->is_alt) {
        KB_ALT= k->released == 0;
        return '\0';
    }

    if(k->is_numeric) {
        //number keys =>
        if(KB_SHIFT == 0) {
            switch (k->rscancode)
            {
                case 0x02: return ('1'); break;
                case 0x03: return ('2'); break;
                case 0x04: return ('3'); break;
                case 0x05: return ('4'); break;
                case 0x06: return ('5'); break;
                case 0x07: return ('6'); break;
                case 0x08: return ('7'); break;
                case 0x09: return ('8'); break;
                case 0x0A: return ('9'); break;
                case 0x0B: return ('0'); break;
                default: return '='; break;
            }
        } else {
            switch (k->rscancode)
            {
                case 0x02: return '!'; break; //1
                case 0x03: return '@'; break; //2
                case 0x04: return '#'; break;
                case 0x05: return '$'; break;
                case 0x06: return '%'; break;
                case 0x07: return '^'; break;
                case 0x08: return '&'; break;
                case 0x09: return '*'; break;
                case 0x0A: return '('; break; //9
                case 0x0B: return ')'; break; //0
                default: return '-'; break;
            }
        }
    }
    if(k->is_minus) {
        if(KB_SHIFT) {
            return '_';
        }
        return '-';
    }
    if(k->is_equals) {
        if(KB_SHIFT) {
            return '+';
        }
        return '=';
    }
    if(k->is_forwardslash) {
        if(KB_SHIFT) {
            return '?';
        }
        return '/';
    }
    if(k->is_open_brace) {
        if(KB_SHIFT) {
            return '{';
        }
        return '[';
    }
    if(k->is_close_brace) {
        if(KB_SHIFT) {
            return '}';
        }
        return ']';
    }
    if(k->is_comma) {
        if(KB_SHIFT) {
            return '<';
        }
        return ',';
    }
    if(k->is_period) {
        if(KB_SHIFT) {
            return '>';
        }
        return '.';
    }
    if(k->is_colon) {
        if(KB_SHIFT) {
            return ':';
        }
        return ';';
    }
    if(k->is_quote) {
        if(KB_SHIFT) {
            return '?';
        }
        return '/';
    }
    if(k->is_pipe) {
        if(KB_SHIFT) {
            return '|';
        }
        return '\\';
    }
    if(k->is_backtick) {
        if(KB_SHIFT) {
            return '~';
        }
        return '`';
    }
    if(k->is_backtick) {
        if(KB_SHIFT) {
            return '~';
        }
        return '`';
    }
    if(k->is_space) {
        return ' ';
    }
    if(k->is_escape) {
        return '\0'; 
    }
    if(k->is_backspace) {
        return '\0';
    }
    if(k->is_return) {
        return '\0';
    }
    if(k->is_tab) {
        return '\0';
    }

    if(k->is_delete) {
        return '\0';
    }
    if(k->is_arrow_left) {
        return '\0';
    }
    if(k->is_arrow_right) {
        return '\0';
    }
    if(k->is_arrow_up) {
        return '\0';
    }
    if(k->is_arrow_down) {
        return '\0';
    }
    return scancode_to_char(k->rscancode);
}

//TODO : convert num keys to numeric with num-lock true
char get_numeric(char scancode) {
    switch (scancode)
    {
    case 0x02: return ((char)1); break;
    case 0x03: return ((char)2); break;
    case 0x04: return ((char)3); break;
    case 0x05: return ((char)4); break;
    case 0x06: return ((char)5); break;
    case 0x07: return ((char)6); break;
    case 0x08: return ((char)7); break;
    case 0x09: return ((char)8); break;
    case 0x0A: return ((char)9); break;
    case 0x0B: return ((char)0); break;
        return 1;
        break;
    default:
        return -1;
        break;
    }
}

char get_is_pad(char scancode) {
    switch (scancode)
    {
    case 0x47:
    case 0x4B:
    case 0x4F:
    case 0x45:
    case 0x48:
    case 0x4C:
    case 0x50:
    case 0x52:
    case 0x46:
    case 0x49:
    case 0x4D:
    case 0x51:
    case 0x53:
    case 0x54:
    case 0x37:
    case 0x4E:
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

char get_scancode_sidedness(char scancode) {
    switch (scancode)
    {
    //LEFT
    case 0x2A: //Shift
    case 0x1D: //Ctrl
    case 0x38: //Alt
        return 0;
    //RIGHT
    case 0x36: //Shift
        return 1;
    default:
        return -1;
        break;
    }
}

void keyboard_callback(registers_t regs)
{
    // fb_move_cursor(0);
    // fb_print_str("Keyboard Event!", 0,1);
    keyboard_event k = read_scan_code();

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    char ch = compute_key(&k);
    
    // char str[9];
    // int_to_hex(k.scancode,str);
    // fb_write_str(162, "          ", 0, 15);
    // fb_write_str(162, str, 0, 15);

    // char strb[9];
    // int_to_hex(k.rscancode,strb);
    // fb_write_str(162+80, "          ", 0, 15);
    // fb_write_str(162+80, strb, 0, 15);

    // char strc[9];
    // int_to_hex(ch,str);
    // fb_write_str(162+80+80, "          ", 0, 15);
    // fb_write_str(162+80+80, strc, 0, 15);
    
    if(ch == 0) { // special key
        if(k.is_backspace) {
            fb_offset_cursor(-1);
            fb_print_cell(' ', 0, 15);
            fb_offset_cursor(-1);
            return;
        }
        if(k.is_return) {
            fb_offset_cursor(FB_NUM_COLUMNS);
            fb_shift_down(fb_get_current_row());
            fb_move_cursor_to_start_of_row();
            return;
        }
        if(k.is_tab) {
            fb_print_str("    ", 0, 15);
            return;
        }
        if(k.is_delete) {
            fb_offset_cursor((signed int)-1);
            fb_print_cell(' ', 0, 15);
            return;
        }
        if(k.is_arrow_left) {
            fb_offset_cursor((signed int)-1);
            return;
        }
        if(k.is_arrow_right) {
            fb_offset_cursor(1);
            return;
        }
        if(k.is_arrow_up) {
            fb_offset_cursor((signed int)-80);
            return;
        }
        if(k.is_arrow_down) {
            fb_offset_cursor(80);
            return;
        }
        if(k.is_escape) {
            // PANIC("ESCAPE WAS PRESSED!");
            switch_page_directory_i(1);    
        }
        return;
    }
    
    //generic alhanumeric key
    if (k.released)
    {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
    } else {
        fb_print_cell(ch,0,15);
    }
    // fb_fprint_str("Keyboard scancode {%d} -> ", (int[]){scancode}, 0,1);
    
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, &keyboard_callback);
}

char scancode_to_char(char scancode) {
    // Regular keys
    if(KB_SHIFT) {
        switch (scancode) {
            case 0x10: return 'Q';
            case 0x11: return 'W';
            case 0x12: return 'E';
            case 0x13: return 'R';
            case 0x14: return 'T';
            case 0x15: return 'Y';
            case 0x16: return 'U';
            case 0x17: return 'I';
            case 0x18: return 'O';
            case 0x19: return 'P';
            case 0x1E: return 'A';
            case 0x1F: return 'S';
            case 0x20: return 'D';
            case 0x21: return 'F';
            case 0x22: return 'G';
            case 0x23: return 'H';
            case 0x24: return 'J';
            case 0x25: return 'K';
            case 0x26: return 'L';
            case 0x2C: return 'Z';
            case 0x2D: return 'X';
            case 0x2E: return 'C';
            case 0x2F: return 'V';
            case 0x30: return 'B';
            case 0x31: return 'N';
            case 0x32: return 'M';
            default: return '\0'; // Default: return -1 for unknown scancodes
        }
    }
    switch (scancode) {
        case 0x10: return 'q';
        case 0x11: return 'w';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x2C: return 'z';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return 'm';
        default: return '\0'; // Default: return -1 for unknown scancodes
    }
}