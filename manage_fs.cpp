#ifndef MANAGE_FS_CPP
#define MANAGE_FS_CPP
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "manage_fs.h"

extern std::fstream iFs;
extern masterBlock mb;

int loadFS(std::string fileName) {  //opens the fs file and loads master block
  iFs.open(fileName, std::ios::out | std::ios::in | std::ios::binary);
  if(!iFs.is_open()) return 1;
  mb = masterBlock(iFs);
  return 0;
}

int makeFS(std::string fileName, size_t givenSize) {  //givenSize in bytes
  iFs.open(fileName, std::ios::binary | std::ios::out);
  if(!iFs.is_open()) {
    std::cout << "Could not create file! Maybe not enough space.\n";
      return 1;
  }
  iFs.seekp(givenSize - 1);
  iFs.write("", 1);
  //created file of given size
  mb.maxByteSize = givenSize;
  mb.totalFiles = 0;
  mb.totalBlocks = (givenSize-5*sizeof(size_t))/(MAX_NAME_LENGTH+sizeof(bool)+3*sizeof(size_t)+BLOCK_SIZE);
  mb.firstBlock = 0; //NOT USED SO FAR
  mb.emptyBlocks = mb.totalBlocks;
  mb.save(iFs);
  iFs.close();
  return 0;
}

int find(std::string fileName) {
  for(int i=0, found=0; found<mb.totalFiles; i++) {
    blockDescriptor temp(iFs, i);
    if(!temp.taken) continue;
    if(strcmp(fileName.c_str(), temp.name)==0) return i;
    found++;
  }
  return -1;
}

int deleteFS(std::string fileName) {
  std::remove(fileName.c_str());
}

void displayFiles() {
  std::vector<std::string> foundNames;
  std::vector<size_t> foundSizes;
  for(int i=0; i<mb.totalBlocks; i++) {
    blockDescriptor temp(iFs, i);
    if(!temp.taken) continue;
    std::string tempString(temp.name);
    //if not present in the vector
    if(std::find(foundNames.begin(), foundNames.end(), tempString)==foundNames.end()) {
      foundNames.push_back(tempString);
      block tempBlock(iFs, i);
      foundSizes.push_back(tempBlock.takenSize);
      while(temp.next!=mb.totalBlocks+1) {
        tempBlock = block(iFs, temp.next);
        foundSizes[foundSizes.size()-1] += tempBlock.takenSize;
        temp = blockDescriptor(iFs, temp.next);
      }
      std::cout << "\n" << foundNames[foundNames.size()-1] << " --- " << foundSizes[foundSizes.size()-1] << " B";
    }
  }
  std::cout << std::endl;
/*  for(int i=0; i<mb.totalBlocks; i++) {
    blockDescriptor temp(iFs, i);
    if(!temp.taken) continue;
      block tempBlock(iFs, i);
      std::cout << tempBlock.takenSize << " ";
    }
    std::cout<< std::endl;
  */
}

void mapFS(std::string fileName) {
  std::cout << "\nTotal size: " << mb.totalBlocks*BLOCK_SIZE << " Bytes\t"
       << "Free size: " << mb.emptyBlocks*BLOCK_SIZE << " Bytes\n"
       << "File count: " << mb.totalFiles << "\n[";
  for(int i=0; i<mb.totalBlocks; i++) {
    blockDescriptor temp(iFs, i);
    if(temp.taken)
      std::cout << "#";
    else
      std::cout << ".";
  }
  std::cout << "]\n";
}

void closeFS() {
    iFs.close();
}

#endif
