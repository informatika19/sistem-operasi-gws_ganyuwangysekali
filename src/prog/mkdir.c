#include "fileio.h"
#include "buffer.h"

int main(){
  char fileBuf[8192], args[512], parent, *argsP;
  int err;
  lib_readFile(fileBuf, "tempc", &err, 0xFF);
  parse(fileBuf, &parent, args);
  argsP = &args;
  while(*argsP == ' ' && *argsP != 0) argsP++;
  if(*argsP == 0){
    print("Usage: mkdir <name>");
  }
  else{
    createFolder(argsP, parent, &err);
    if(err < 0){
      printInt(err);
      print("Error!");
    }
  }
  exec("/bin/shell", 0xFF, &err);
}