#include "progs.h"
#include "stds.h"

/*
char *getcwd(char *buf)
{
	int errno = interrupt(0x21, 0x4702, buf, 0x101, 0x0001);
	
	// reference for error code :
	// bbc.nvg.org/doc/Master%20512%20Technical%20Guide/m512techb_int21.htm
	if (errno == 4 || errno == 6)
	{
		errno = interrupt(0x21, 0x4702, buf, 0x102, 0x0001);
	}
	
	if (errno == 4 || errno == 6)
	{
		return NULL;
	}
	return buf;
	// KONDISI YANG DIHARAPKAN :
	// buf isinya adalah 1 dari sektor file
}
*/

/* ubah byte terakhir sector menjadi parent dari cwd */
char *getcwd(char *buf)
{
	int curDirIndex = sector[511];
	if (curDirIndex == 0xFF) // root
	{
		return NULL;
	}
	// strcpy(buf, &files[16 * curDirIndex], 16);
	buf = files[16 * curDirIndex];
	return buf;
}
