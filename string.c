#include "string.h"
#include "common.h"
int strlen(const char *str)
{
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}


void strreverse(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}


void int_to_hex(int value, char* hexString) {
    // Define the characters for hexadecimal representation
    const char hexChars[] = "0123456789ABCDEF";

    // Ensure that the integer value fits within a 32-bit signed integer
    unsigned int uValue = (unsigned int)value;

    // Iterate over each nibble (4 bits) in reverse order
    for (int i = 7; i >= 0; i--) {
        // Extract the current nibble
        unsigned char nibble = (uValue >> (4 * i)) & 0xF;
        
        // Append the corresponding hexadecimal character to the string
        *hexString++ = hexChars[nibble];
    }

    // Null-terminate the string
    *hexString = '\0';
}


/// @brief  EXAMPLE:
/// int number = 1024;
///    char buffer[12];  // Assuming a maximum of 12 digits for the integer
///    int_to_str(number, buffer);
///    // Now, 'buffer' contains the string representation of 'number'
/// @param num 
/// @param buffer 
void int_to_str(int num, char* buffer) {
    int i = 0;
    int is_negative = 0;

    // Handle negative numbers
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Process individual digits
    do {
        buffer[i++] = (num % 10) + '0';
        num = num / 10;
    } while (num != 0);

    // Add negative sign if applicable
    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0'; // Null-terminate the string

    // Reverse the string to get the correct order of digits
    strreverse(buffer, i);
}

/// @brief Copy a slice from <from> to <to> of string str into destination out;
///        Makes no presumptions on the allocated space of out;
/// @param str 
/// @param from 
/// @param to 
/// @param out 
void strslice(char str[], int from, int to, char *out)
{
    int i, j;

    for (i = from, j = 0; i <= to && str[i] != '\0'; i++, j++)
    {
        out[j] = str[i];
    }

    out[j] = '\0'; // Null-terminate the output string
}


// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      // why did the loop exit?
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src)
{
    do
    {
      *dest++ = *src++;
    }
    while (*src != 0);
    return *dest;
}


/// @brief write string of length <length> starting from <from> source string <src> onto the <offset> position of string <dest>  
///         modifies in place dest; does not overflow dest;
/// @param dest the string to copy into
/// @param src the string to copy from
/// @param from the offset of string srd to copy from
/// @param length the length of characters (or null terminate) to copy
/// @param offset the offset to write from
void strw_flo(char *dest, char *src, int from, signed int length,signed int offset)
{
    *src--;

    while (offset)
    {
        *dest++;
        offset--;
    };
    while (from);
    {
        *src++;
        from--;
    } 
    do
    {
      *dest++ = *src++;
      length--;
    }
    while (length && (*src != 0 || *dest != 0));
    *dest = 0; // NULL TERMINATE
}

// Concatenate the NULL-terminated string append onto
// the end of dest, and return new string.
void strcat(char *dest, char *append, char* buffer)
{
    strw_flo(buffer,dest,0,strlen(dest),0);
    strw_flo(buffer,append,0,strlen(append),strlen(dest));
    buffer[strlen(dest)+strlen(append)+1] = 0;
}