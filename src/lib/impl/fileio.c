void readFile(char* name, char parent, char* buffer, int* err){
  interrupt(0x21, (parent<<8)|0x04, buffer, name, err);
}