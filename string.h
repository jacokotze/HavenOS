#ifndef INCLUDE_STRING_H
#define INCLUDE_STRING_H

int strlen(const char *str);
void strreverse(char* str, int length);
void int_to_str(int num, char* str);
void strslice(char str[], int from, int to, char *out);
int strcmp(char *str1, char *str2);
int strncmp(const char *s1, const char *s2, int n);
// void strcpy(char in[], char into[], int from, int to, int offset);
void int_to_hex(int value, char* hexString);
void strcat(char *dest, char *append, char* buffer);
void strw_flo(char *dest, char *src, int from, int length, int offset);

#define COUNT(arr)
#endif