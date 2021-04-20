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
    print("Usage: mkdir <name>\n");
  }
  else{
    createFolder(argsP, parent, &err);
    if(err == -1)
    {
      print("File or directory exists!\n");
    }
    else if(err == -2)
    {
      print("Empty your storage!\n");
    }
    else if(err == -4)
    {
      print("No such directory!\n");
    }
    else if(err == -5)
    {
      print("You can't make a directory inside a file!\n");
    }
  }
  exec("/bin/shell", 0xFF, &err);
}