#include "string.h"
#include "fileio.h"
#include "buffer.h"

void copy(char* args, char parentIndex);

int main(){
	char buffer[512 << 4];
	char parent, arg[512];
	int err;

	lib_readFile(buffer, "tempc", &err, 0xFF);
	removeFEntry("tempc", 0xFF, &err);
	parse(buffer, &parent, arg);

	copy(arg, parent);

	clear(buffer, 8192);
	buffer[0] = parent;
	err = 1;
	lib_writeFile(buffer, "tempc", &err, 0xFF);

	exec("/bin/shell", 0xFF, &err);
}

void copy(char* args, char parentIndex)
{
    int len;
	char content[8192], dir[128], inputPath[128], outputPath[128];
	unsigned char valid = 0, isRecursive = 0;

	clear(inputPath, 128);
	clear(outputPath, 128);
	while(*args != 0)
	{
        if(*args == ' ')
        {
            args++;
            continue;
        }
		len = 0;
		clear(dir, 128);
		while(*(args + len) != ' ' && *(args + len) != 0) len++;
		strncpy(dir, args, len);
		if(strncmp(dir, "-r", 2) == 0) isRecursive = 1;
		else if(strlen(inputPath) == 0) strncpy(inputPath, dir, 128);
		else if(strlen(outputPath) == 0) strncpy(outputPath, dir, 128);
		else
		{
			len = -1;
			break;
		}
		valid++;
		args += len;
	}

	if(len == -1 || valid > 3 || valid < 2)
	{
		print("Usage: cp [-r] <input> <output>\n");
		return;
	}

	/* Mengolah path input */
	clear(content, 8192);
	lib_readFile(content, inputPath, &len, parentIndex);
	if(len == -1) // file tidak ditemukan
	{
		print("No such file or directory\n");
		return;
	}

	if(!isRecursive && len == -2){
		print("-r not specified. Not allowed for directory\n");
		return;
	}
	
	// yang kebaca pasti file
	if(!isRecursive)
	{
		lib_writeFile(content, outputPath, &len, parentIndex);
		if(len == -1)
		{
			print("File exists\n");
		}
		else if(len == -2 || len == -3)
		{
			print("Not enough space!\n");
		}
		else if(len < 0)
		{
			print("No such file or directory!\n");
		}
	}
	else
	{
		print("cp -r not implemented\n");
	}
}
