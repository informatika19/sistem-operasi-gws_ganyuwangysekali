char **parse(char *s, char *delim)
{
	char **result;
	char *space;
	char *tmpS;
	while(*s != '\0')
	{
		// ignore blank
		while(*s == ' ')
		{
			s++;
		}
		tmpS = s;
		space = strchr(s, (int)' ');
		while(tmpS != space)
		{
			// masukin jadi result[i]
			tmpS++;
		}
		*s = tmpS;
	}
	return result;
}
