#ifndef MANAGE_BLOCKS_CPP
#define MANAGE_BLOCKS_CPP
#include <iostream>
#include <fstream>
#include "options.h"
#include "manage_fs.h"
#include "manage_blocks.h"

#define NIL_BLOCK (mb.totalBlocks+1)

extern std::fstream iFs;
extern masterBlock mb;

int add(std::string fileName, bool overwrite) {
  //check if file exists in the same folder

  std::ifstream inFile(fileName, std::ifstream::ate | std::ifstream::binary);
  if(!inFile.is_open()) {
    std::cout << "File doesn't exist! Make sure it's in the same folder as fs.\n";
    return 1;
  }
  //check name length
  if(fileName.length()>MAX_NAME_LENGTH) {
    std::cout << "File name too long! Maximum is " << MAX_NAME_LENGTH-1 << " characters." << std::endl;
    return 1;
  }
  //overwrite if needed or exit if name is not unique
  int foundPos = find(fileName);
  if(foundPos!=-1) {
    if(overwrite) {
      removeFile((size_t)foundPos);
    } else {
      std::cout << "File with this name already exists!\n";
      return 1;
    }


  }
  //check if enough space
  size_t fileToAddSize = (size_t) inFile.tellg(); //get file size
  size_t neededBlocks = fileToAddSize/BLOCK_SIZE+1;
  if(mb.emptyBlocks<neededBlocks) {
    std::cout << "Not enough space in fs!\n";
    return 1;
  }
  //actually add file
//  std::cout << "adding file size: " << fileToAddSize << std::endl;
//  std::cout << "needed blocks: " << fileToAddSize/BLOCK_SIZE+1 << std::endl;
//  std::cout << "\nnext empty: " << findNextEmpty();
  inFile.clear();
  inFile.seekg(0);
  char buffer[BLOCK_SIZE];
  size_t contArray[neededBlocks];
  contArray[0] = findNextEmpty();

  for(int i=1; i<neededBlocks; i++) {
    contArray[i] = findNextEmpty(contArray[i-1]+1);
  }
  for(int i=0; i<neededBlocks; i++) {
    inFile.read(buffer, BLOCK_SIZE); //read enough memory to start copying
    blockDescriptor temp; //getting BD and block to fill
    block tempBlock;
    //fill with data
    temp.taken = true;
    strncpy(temp.name, fileName.c_str(), MAX_NAME_LENGTH);
    memcpy(tempBlock.data, buffer, BLOCK_SIZE);
    tempBlock.takenSize = (fileToAddSize>BLOCK_SIZE) ? BLOCK_SIZE : fileToAddSize;
    fileToAddSize -= BLOCK_SIZE;
    if(i==neededBlocks-1) {
      tempBlock.next = NIL_BLOCK;
      temp.next = NIL_BLOCK;
    } else {
      tempBlock.next = contArray[i+1];
      temp.next = contArray[i+1];
    }

    temp.savep(iFs, contArray[i]);
    tempBlock.savep(iFs, contArray[i]);
  }
  mb.emptyBlocks -= neededBlocks;
//  std::cout << "\nemptyblock: " << mb.emptyBlocks;
  mb.totalFiles++;
//  std::cout << "  totalfiles: " <<mb.totalFiles;
  return 0;
}

size_t findNextEmpty(size_t position) {
  for(size_t i=position; i<mb.totalBlocks; i++) {
    blockDescriptor temp(iFs, i);
    if(!temp.taken) return i;
  }
  return NIL_BLOCK;
}

int grab(std::string fileName) {

  int foundPos = find(fileName);
  if(foundPos==-1) {
    std::cout << "No such file in fs!\n";
    return 1;
  }
  std::ofstream outFile;
  outFile.open(fileName, std::ios::out | std::ios::binary);
  //std::cout << mb.totalFiles;
  if(!outFile.is_open()) {
    std::cout << "Could not create file!\n";
    return 1;
  }

  //char buffer[BLOCK_SIZE];
  block tempBlock(iFs, foundPos);
  while(true) {
    outFile.write((char*) tempBlock.data, tempBlock.takenSize);
    if(tempBlock.next == NIL_BLOCK) {
      outFile.close();
      return 0;
    }
    tempBlock = block(iFs, tempBlock.next);
  }
  return 1;
}

int removeFile(size_t firstBlockNumber) {
  blockDescriptor temp(iFs, firstBlockNumber);
    mb.emptyBlocks++;
    temp.taken = false;
    temp.savep(iFs, firstBlockNumber);
    if (temp.next != NIL_BLOCK)
      removeFile(temp.next);
    else
      mb.totalFiles--;
  return 0;
}

#endif
