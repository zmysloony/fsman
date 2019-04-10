#include <iostream>
#include <getopt.h>
#include "options.h"
#include "manage_fs.h"
#include "manage_blocks.h"


std::fstream iFs;
masterBlock mb;

int main(int argc, char **argv) {
/*  makeFS("lol", 50000);
  loadFS("lol");

  std::cout << mb.maxByteSize << " ";
  std::cout << mb.totalBlocks << " ";
  std::cout << mb.emptyBlocks << " ";
  std::cout << mb.totalFiles << " ";
  std::cout << mb.firstBlock << " " << std::endl;
  blockDescriptor newb;
  newb.taken = 1;
  strncpy(newb.name, "lolzers", MAX_NAME_LENGTH);
  newb.next = mb.totalBlocks+1;
  block testblock;
  strncpy(testblock.data, "asdasdwedf3w", BLOCK_SIZE);
  testblock.takenSize = 13;
  testblock.next = 12;
  testblock.savep(iFs,9);
  iFs.close();

  iFs.open("lol",  std::ios::out | std::ios::in | std::ios::binary);
  mb.totalFiles=1;
  mb.emptyBlocks--;
  mb.save(iFs);
  newb.savep(iFs,9);
//  oFs.seekp(0, std::ios::end);
  iFs.close();
  iFs.open("lol",  std::ios::out | std::ios::in | std::ios::binary);
  blockDescriptor lolw(iFs, 9);

  std::cout << lolw.taken << " " << lolw.name << " " << lolw.next << "\n";
  displayFiles();

  removeFile(9);
  //std::cout << "next?: " << std::endl;
  add("wtf", false);
  add("wtf2", false);
  add("wtf3", false);
  add("wtf4", false);
  add("wtf5", false);
  add("wtf6", false);
  add("wtf7", false);
  add("wtf8", false);
  add("wtf9", false);
  add("wtf10", false);
  add("wtf11", false);

  add("wtf11", false);
  removeFile(1);
  displayFiles();
  mb.save(iFs);
  iFs.close();
*/
  if(argc==1) {
    std::cout << "No arguments! Run with --help to see manual.\n";
    return 0;
  }
  if(argc>1 && strcmp(argv[1],"--help")==0) {
    std::cout << "How to use:\n"
              << "\t\"-f <fs_name>\" option is required with every command\n"
              << "\t-m \t\t: displays a Map of given filesystem\n"
              << "\t-l \t\t: displays a List of files in a fs\n"
              << "\t-c <size> \t: Creates a fs of given size (in bytes)\n"
              << "\t-d \t\t: Deletes a fs\n"
              << "\t-a <file_name>\t: Adds (if possible) a file to fs\n"
              << "\t\t\t with option [-o] to overwrite file if present\n"
              << "\t-r <file_name>\t: Removes (if present) a file from a fs\n"
              << "\t-g <file_name>\t: Grabs a file from fs and saves on a default filesystem\n";
    return 0;
  }

  int opt;
  bool enableOverwrite = false;
  bool noFileName = true;
  int option = -100;
  int size = 5000;
  std::string fsName;
  std::string fileName;

  while ((opt = getopt(argc,argv,"a:r:oc:dg:mf:l")) != EOF) {
    switch(opt) {
              case 'o':
                enableOverwrite = true;
                break;
              case 'f':
                noFileName = false;
                fsName = optarg;
                break;
              case 'l':
                option = 'l';
                break;
              case 'd':
                option = 'd';
                break;
              case 'c':
                option = 'c';
                size = atoi(optarg);
                break;
              case 'a':
                option = 'a';
                fileName = optarg;
                break;
              case 'm':
                option = 'm';
                break;
              case 'g':
                option = 'g';
                fileName = optarg;
                break;
              case 'r':
                option = 'r';
                fileName = optarg;
                break;
              case '?':
                std::cout<< "\nUsage:\n\tfsman -c <fs size> -f <fs name> : creates an fs" <<
                             "\n\t\t-d -f <fs name> : deletes an fs" <<
                             "\n\t\t-m -f <fs name> : displays a memory map" <<
                             "\n\t\t[-o] -a <file name> -f <fs name> : adds a file to filesystem" <<
                             "\n\t\t\t with -o to enable overwrite" <<
                             "\n\t\t-r <file name> -f <fs name>: removes a file from an fs\n";
                return 1;
              default: std::cout<<std::endl; return 1;
    }
  }

  if(noFileName) {
    std::cout << "No fs name given!";
    return -1;
  }

  switch(option) {
    case 'd':
      deleteFS(fsName);
      break;
    case 'm':
      if(loadFS(fsName)!=0) {
        std::cout << "No such fs!\n";
        return 1;
      }
      mapFS(fsName);
      break;
    case 'c':
      makeFS(fsName, size);
      break;
    case 'l':
      if(loadFS(fsName)!=0) {
        std::cout << "No such fs!\n";
        return 1;
      }
      displayFiles();
      closeFS();
      break;
    case 'a':
      if(loadFS(fsName)!=0) {
        std::cout << "No such fs!\n";
        return 1;
      }
      add(fileName, enableOverwrite);
      mb.save(iFs);
      break;
    case 'g':
      if(loadFS(fsName)!=0) {
        std::cout << "No such fs!\n";
        return 1;
      }
      grab(fileName);
      mb.save(iFs);
      break;
    case 'r':
      if(loadFS(fsName)!=0) {
        std::cout << "No such fs!\n";
        return 1;
      }
      int toRemove = find(fileName);
      if(toRemove==-1) {
        std::cout << "File not found in fs!\n";
        closeFS();
        return 1;
      }
      removeFile(toRemove);
      mb.save(iFs);
      break;
  }
  closeFS();
  return 0;
}
