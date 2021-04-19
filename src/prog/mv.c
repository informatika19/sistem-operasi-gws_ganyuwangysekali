#include "fileio.h"
#include "buffer.h"
#include "string.h"

int main(){
  char fileBuf[8192], args[512], parent, from[256], fromParent, to[256], toParent, newName[256];
  char *dir, *argsP, *fromP, *toP;
  int err;
  dir = fileBuf;
  argsP = args;
  fromP = from;
  toP = to;
  lib_readFile(fileBuf, "tempc", &err, 0xFF);
  parse(fileBuf, &parent, args);
  while(*argsP == ' ' && *argsP != 0) argsP++;
  while(*argsP != ' ' && *argsP != 0){
    *fromP = *argsP;
    fromP++;
    argsP++;
  }
  *fromP = 0;
  while(*argsP == ' ' && *argsP != 0) argsP++;
  while(*argsP != ' ' && *argsP != 0){
    *toP = *argsP;
    toP++;
    argsP++;
  }
  *toP = 0;
  if(strlen(from) == 0 || strlen(to) == 0){
    print("Usage: mv <src> <dest>");
  }
  else{
    lib_getFileName(to, newName);
    lib_getAbsolutePath(to, to);
    fromParent = getParent(from, parent);
    toParent = getParent(to, parent);
    if(fromParent == 0xFE){
      print("source not found!");
    }
    if(toParent == 0xFE){
      print("destination folder not found");
    }
    else{
      lib_readSector(dir, 0x101);
      lib_readSector(dir+512, 0x102);
      dir[fromParent<<4] = toParent;
      strncpy(dir+(fromParent<<4)+2, newName, 14);
      lib_writeSector(dir, 0x101);
      lib_writeSector(dir+512, 0x102);
    }
  }
  exec("/bin/shell", 0xFF, &err);
}