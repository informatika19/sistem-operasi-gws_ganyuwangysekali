#define bufsize 20

void printString(char *string);
void readString(char *string);
void clear(char *buffer, int length);

int main () {
	char buffer[bufsize];
	// set graphics mode
	interrupt(0x10, 0x0004, 0x0000, 0x0000, 0x0000);
	// gambar :D
	interrupt(0x10, 0x0C0E, 0x0000, 5, 5);
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