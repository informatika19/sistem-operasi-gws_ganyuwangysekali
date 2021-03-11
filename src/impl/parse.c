char **parse(char *s, char *delim)
{
	char *result[255];
	char *space;
	char *tmpS;
	int numOfCommand = 0;
	while(*s != '\0')
	{
		// ignore blank
		while(*s == ' ')
		{
			s++;
		}
		tmpS = s;
		space = strchr(s, *delim);
		while(tmpS != space)
		{
			// masukin jadi result[i]
			*(result[numOfCommand]) = tmpS;
			(result[numOfCommand])++;
			tmpS++;
		}
		numOfCommand++;
		*s = tmpS;
	}
	return result;
}
