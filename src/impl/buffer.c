#include "buffer.h"

void clear(char* buffer, int length){
	int i;
	for(i = 0; i < length; i++){
		buffer[i] = 0;
	}
}