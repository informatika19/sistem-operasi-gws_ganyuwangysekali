#include "progs.h"
#include "stds.h"

char *getcwd(char *buf)
{
	if (cwdIdx == 0xFF) // root
	{
		return NULL;
	}
	
	char dir[1024];
	readSector(dir, 0x101);
	readSector(dir + 512, 0x102);
	
	strncpy(buf, dir + (cwdIdx << 4), 16);
	return buf;
}
