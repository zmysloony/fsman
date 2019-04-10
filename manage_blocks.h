#ifndef MANAGE_BLOCKS_H
#define MANAGE_BLOCKS_H
#include <iostream>
int add(std::string fileName, bool overwrite);
size_t findNextEmpty(size_t position=0);
int grab(std::string filename);
int removeFile(size_t firstBlockNumber);
#endif
