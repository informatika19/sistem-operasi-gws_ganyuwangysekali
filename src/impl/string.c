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

char strncmp(char *str1, char *str2, int n){
	while(*str1 != 0 && *str2 != 0 && n > 0){
		if(*str1 == *str2){
			str1++;
			str2++;
			n--;
		}
		else return 1;
	}

	if(n <= 0 || *str1 == *str2) return 0;
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

void strcpy(char* s1, char *s2)
{
	while(*s2 != '\0')
	{
		*s1 = *s2;
		s1++;
		s2++;
	}
	// s2 == '\0'
	*s1 = '\0';
	return;
}

void strncpy(char *s1, char *s2, int n){
	*s1 = *s2;
	while(*s1 != 0 && n > 0){
		s1++;
		s2++;
		n--;
		*s1 = *s2;
	}
	*s1 = '\0';
	return;
}

char *strncat(char *s1, char *s2, int n)
{
	char* s = s1;
	while(*s != '\0')
	{
		s++;
	}
	strncpy(s, s2, n);
	return s1;
}
/*** IMPLEMENTASI STRING.H ***/
