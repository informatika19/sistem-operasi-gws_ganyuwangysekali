#define bufsize 20

#define VGA_MEMORY_BASE 0xA000
#define VGA_WIDTH 320
#define VGA_HEIGHT 200

#define TEXT_MEMORY_BASE 0xB000
#define TEXT_OFFSET 0x8000

void printString(char *string);
void readString(char *string);
void clear(char *buffer, int length);

extern unsigned char logo[];

int main () {
	char buffer[bufsize];
	int x, y;
	int width = logo[0];
	int height = logo[1];
	int startx = (VGA_WIDTH - width)/2;
	int starty = (VGA_HEIGHT - height)/2;
	// set graphics mode
	// http://www.oldlinux.org/Linux.old/docs/interrupts/int-html/rb-0069.htm
	// 13h = VGA Graphics (320x200, 256 colors)
	interrupt(0x10, 0x0013, 0x0000, 0x0000, 0x0000);
	// render graphics
	for(y = width; y > 0; y--){
		for(x = width; x > 0; x--){
			putInMemory(VGA_MEMORY_BASE, (y+starty) * VGA_WIDTH + (x+startx), logo[(y*width)+x]);
		}
	}

	makeInterrupt21();
	while(1){
		clear(buffer, bufsize);
		interrupt(0x21, 0x01, buffer, 0x0000, 0x0000);
		interrupt(0x21, 0x00, buffer, 0x0000, 0x0000);
	}
}

void handleInterrupt21 (int AX, int BX, int CX, int DX) {
	switch(AX){
		case 0x0:
			printString(BX);
			break;
		case 0x1:
			readString(BX);
			break;
		default:
			printString("invalid interrupt");
	}
}

void printString(char *string){
	int i = 0;
	// set mode text 
	interrupt(0x10, 0x0304, 0x0000, 0x0000, 0x0000);
	for(i = 0; string[i] != 0; i++){
		// set posisi kursor
		interrupt(0x10, 0x0200, 0x0000, 0x0000, 0x0000+i);
		// tulis string :D
		interrupt(0x10, 0x0900 + string[i], 0x000D, 0x0001, 0x0000);
	}
}

void readString(char *string){
	int length = 0;
	char current = interrupt(0x16, 0x0000, 0x0000, 0x0000, 0x0000);
	while(length < bufsize-1 && current != 0x0D){
		string[length] = current;
		length++;
		current = interrupt(0x16, 0x0000, 0x0000, 0x0000, 0x0000);
	}
	string[length] = 0;
}

void clear(char* buffer, int length){
	int i;
	for(i = 0; i < length; i++){
		buffer[i] = 0;
	}
}