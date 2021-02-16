int main () {
	char* HelloWorld = "Hello, world!";
	int i = 0, j;
	for(; i < 13; i++){
		j = i<<1;
		putInMemory(0xB000, 0x8000+j, HelloWorld[i]);
		putInMemory(0xB000, 0x8001+j, 0xD);
	}
	makeInterrupt21();
	interrupt(0x10, 0x0C0D, 0x0000, 0x0000, 0x0000); 
	while (1);
}

void handleInterrupt21 (int AX, int BX, int CX, int DX) {
	interrupt(0x10, AX, BX, CX, DX);
}
