#include "fileio.h"

void ls(char* arg, char parentIndex);
void parse(char* cmd, char* parent, char* arg);

int main()
{
	char buffer[512<<4];
	char parent, arg[512];
	int err;
	lib_readFile(buffer, "tempc", &err, 0xFF);
	removeFEntry("tempc", 0xFF, &err);
	parse(buffer, &parent, arg);
	ls(arg, parent);
	exec("/bin/shell", 0xFF, &err);
}

void parse(char* cmd, char* parent, char* arg){
	*parent = *cmd;
	cmd++;
	while(*cmd == ' ') cmd++; // ignore leading spaces
	while(*cmd != ' ' && *cmd != 0) cmd++; // ignore arg[0]
	while(*cmd == ' ') cmd++; // ignore space between arg[0] and arg[1]
	// copy arg[1] to arg
	while(*cmd != 0){
		*arg = *cmd;
		arg++;
		cmd++;
	}
	*arg = 0;
}

void ls(char *arg, char parentIndex)
{
	char dir[1024];
	char current[16];
	int i;
	lib_readSector(dir, 0x101);
	lib_readSector(dir + 512, 0x102);

	if(*arg != 0) parentIndex = getParent(arg, parentIndex);

	if(parentIndex != 0xFF && dir[(parentIndex << 4) + 1] > 0x20 && dir[(parentIndex << 4)+1] != 0xFF)
	{
		parentIndex = dir[(parentIndex << 4) + 1];
	}
	
	for(i = 0; i < 0x40; i++)
	{
		// parent dari isifile = direktori sekarang
		if (dir[i << 4] == parentIndex && dir[(i << 4) + 2] != 0)
		{
			clear(current, 16);
			strncpy(current, dir+(i << 4) + 2, 14);
			current[strlen(current)] = '\n';
			print(current);
		}
	}
}
