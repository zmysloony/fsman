#ifndef MANAGE_FS_H
#define MANAGE_FS_H
#include <iostream>
#include <fstream>
#include "options.h"


int loadFS(std::string fileName);
int makeFS(std::string fileName, size_t givenSize);
int find(std::string fileName);
int deleteFS(std::string fileName);
void displayFiles();
void mapFS(std::string fileName);
void closeFS();
#endif
