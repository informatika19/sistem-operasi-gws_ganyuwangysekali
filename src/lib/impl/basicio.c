void print(char* buf){
  interrupt(0x21, 0, buf, 0, 0);
}

void printInt(int x){
  char current[2];
  int y, i;
  clear(current, 2);
  if(x < 0) print("-");
  x = abs(x);
  while(x > 9){
    y = x;
    i = 0;
    while(y > 9) { y/=10; i++; }
    current[0] = '0'+(y&0xFF);
    print(current);
    x -= y*(pow(10,i));
  }
  current[0] = '0'+(x&0xFF);
  print(current);
}

void printHex(unsigned int x){
  char current[2];
  unsigned int y, i;
	current[1] = 0;
  print("0x");
  while(x > 0xF){
    y = x;
    i = 0;
    while(y > 0xF) { y /= 0x10; i++; }
    y &= 0xF;
    current[0] = ( y < 10 ? '0' : 'A'-10 )+(y);
    print(current);
    x -= y*pow(0x10, i);
  }
  x &= 0xF;
  current[0] = ( x < 10 ? '0' : 'A'-10 )+(x);
  print(current);
}