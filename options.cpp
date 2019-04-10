#ifndef OPTIONS_CPP
#define OPTIONS_CPP

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include "options.h"

extern masterBlock mb;

masterBlock::masterBlock() {
  maxByteSize = 0;
  totalBlocks = 0;
  firstBlock = 0;
  emptyBlocks = 0;
  totalFiles = 0;
}

masterBlock::masterBlock(std::fstream& fsFile) {
  char buffer[MAX_NAME_LENGTH];
  fsFile.read(buffer, sizeof(size_t));
  maxByteSize = *(size_t*)buffer;
  fsFile.read(buffer, sizeof(size_t));
  totalBlocks = *(size_t*)buffer;
  fsFile.read(buffer, sizeof(size_t));
  firstBlock = *(size_t*)buffer;
  fsFile.read(buffer, sizeof(size_t));
  emptyBlocks = *(size_t*)buffer;
  fsFile.read(buffer, sizeof(size_t));
  totalFiles = *(size_t*)buffer;
}

int masterBlock::save(std::fstream& fsFile) {
  fsFile.clear();
  fsFile.seekp(0, std::ios::beg);
  fsFile.write((char*) &maxByteSize, sizeof(size_t));
  fsFile.write((char*) &totalBlocks, sizeof(size_t));
  fsFile.write((char*) &firstBlock, sizeof(size_t));
  fsFile.write((char*) &emptyBlocks, sizeof(size_t));
  fsFile.write((char*) &totalFiles, sizeof(size_t));

  return 0;
}

blockDescriptor::blockDescriptor()  {
  taken = false;
  next = 0;
}

blockDescriptor::blockDescriptor(std::fstream& fsFile, size_t position) {
  fsFile.seekg(MB_SIZE+position*BD_SIZE, std::ios::beg);
  //std::cout << "write be4: " << fsFile.tellg() << std::endl;
  char buffer[MAX_NAME_LENGTH];
  fsFile.read(buffer, sizeof(bool));
  taken = *(bool*)buffer;
  fsFile.read(buffer, MAX_NAME_LENGTH*sizeof(char));
  strncpy(name, buffer, MAX_NAME_LENGTH);
  fsFile.read(buffer, sizeof(size_t));
  next = *(size_t*)buffer;
//  std::cout << "write after: " << fsFile.tellg() << std::endl;
}

int blockDescriptor::savep(std::fstream& fsFile, size_t position) {
  fsFile.seekp(MB_SIZE+position*BD_SIZE, std::ios::beg);
//  std::cout << "seek save be4: " << fsFile.tellp() << std::endl;
  fsFile.write((char*) &taken, sizeof(bool));
  fsFile.write((char*) name, MAX_NAME_LENGTH*sizeof(char));
  fsFile.write((char*) &next, sizeof(size_t));
  //std::cout << "seek save after: " << fsFile.tellp() << std::endl;
}

block::block() {
  takenSize = 0;
  next = 0;
}

block::block(std::fstream& fsFile, size_t position) {
  fsFile.seekg(MB_SIZE+mb.totalBlocks*BD_SIZE+position*BLOCK_NEXT, std::ios::beg);
  char buffer[BLOCK_SIZE];
  fsFile.read(buffer, sizeof(size_t));
  takenSize = *(size_t*)buffer;
  fsFile.read(buffer, BLOCK_SIZE);
  memcpy((void*)data, (void*)buffer, BLOCK_SIZE);
  fsFile.read(buffer, sizeof(size_t));
  next = *(size_t*)buffer;
}

int block::savep(std::fstream& fsFile, size_t position) {
  fsFile.seekp(MB_SIZE+mb.totalBlocks*BD_SIZE+position*BLOCK_NEXT, std::ios::beg);
  fsFile.write((char*) &takenSize, sizeof(size_t));
  fsFile.write((char*) data, BLOCK_SIZE);
  fsFile.write((char*) &next, sizeof(size_t));
}
#endif
