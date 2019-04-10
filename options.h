#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstring>

#define BLOCK_SIZE 4096
#define MAX_NAME_LENGTH 256
#define MB_SIZE (5*sizeof(size_t))
#define BD_SIZE (MAX_NAME_LENGTH+sizeof(bool)+sizeof(size_t))
#define BLOCK_NEXT (BLOCK_SIZE+2*sizeof(size_t))

class masterBlock {
public:
  size_t maxByteSize;
  size_t totalBlocks;
  size_t firstBlock;  //first block position [in bytes]
  size_t emptyBlocks;
  size_t totalFiles;

  masterBlock();
  masterBlock(std::fstream& fsFile);
  int save(std::fstream& fsFile);
};

class blockDescriptor {
public:
  bool taken;
  char name[MAX_NAME_LENGTH];
  size_t next;

  blockDescriptor();
  blockDescriptor(std::fstream& fsFile, size_t position);
  int savep(std::fstream& fsFile, size_t position = 0);
};

class block {
public:
  size_t takenSize;
  char data[BLOCK_SIZE];
  size_t next;

  block();
  block(std::fstream& fsFile, size_t position);
  int savep(std::fstream& fsFile, size_t position = 0);
};
#endif
