void cat(char *inputPath, char parentIndex)
{
	char content[8192];
	int errno;
	while(*(inputPath) == ' ') inputPath++;
	if(*inputPath == 0){
		interrupt(0x21, 0, "Usage: cat <filename>\n", 0, 0);
		return;
	}
//	readFile(content, inputPath, &errno, parentIndex);
	interrupt(0x21, (parentIndex << 8) | 0x04, content, inputPath, &errno);
	if(errno == -1) // file not found
	{
		interrupt(0x21, 0, "No such file or directory\n", 0, 0);
	}
	else if(errno == -2)
	{
		interrupt(0x21, 0, "Is a directory\n", 0, 0);
	}
	else
	{
		interrupt(0x21, 0, content, 0, 0);
		interrupt(0x21, 0, "\n", 0, 0);
	}
	return;
}

int main()
{
	int argc;
	char* argv;
}