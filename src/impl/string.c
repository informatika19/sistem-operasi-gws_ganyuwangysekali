#include "stds.h"

/*** IMPLEMENTASI STRING.H ***/
int strlen(char *str) {
	int count = 0;
	while (*str != '\0') {
		count++;
		str++;
	}
	return count;
}

char strcmp(char *str1, char *str2)
{
	while(*str1 != '\0' && *str2 != '\0')
	{
		if(*str1 == *str2)
		{
			str1++;
			str2++;
		}
		else
		{
			return 1;
		}
	}
	
	if(*str1 == *str2) return 0;
	return 1;
}

char *strchr(char *s, char c)
{
	while (*s != c && *s != '\0')
	{
		s++;
	}
	
	return (*s == c) ? s : NULL;
}

char *strcpy(char* s1, char *s2, int size)
{
	int num = 0;
	while(num < size && *s2 != '\0')
	{
		*s1 = *s2;
		num++;
		s1++;
		s2++;
	}
	*s1 = '\0';
	return s1;
}

char *strcat(char *s1, char *s2, int size)
{
	int num = 0;
	char* s = s1;
	while(*s != '\0')
	{
		s++;
	}
	strcpy(s, s2, size);
	return s1;
}
/*** IMPLEMENTASI STRING.H ***/
