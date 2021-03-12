#ifndef __STRING_H_
#define __STRING_H_

int strlen(char *str);
char strcmp(char *str1, char *str2);
char strncmp(char *str1, char *str2, int n);
char *strchr(const char *s, int c);
void strcpy(char *s1, const char *s2);
void strncpy(char *s1, const char *s2, int n);
char *strncat(char *s1, const char *s2, int n);

#endif
