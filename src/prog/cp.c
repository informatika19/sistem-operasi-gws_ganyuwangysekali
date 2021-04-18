#include "string.h"
#include "fileio.h"

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
		args += len + 1;
	}
	if((valid > 3) || (len == -1) || (valid < 2))
	{
		print("Usage: cp [-r] <input> <output>\n");
		return;
	}

	/* Mengolah path input */
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
		else
		{
			print("No such file or directory!\n");
		}
    }
	else
	{
		print("cp -r not implemented\n");
	}
}

// perlu pakai getFileName dari file
int main(){

}