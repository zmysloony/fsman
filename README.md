# fsman
A Linux-based virtual file system, using blockchain allocation with static block size (4096 bytes by default). Guarantees no external fragmentation at a cost of heavy internal fragmentation because of static size of data blocks.

Used to store a file system inside another file: at the beginning of said file we have a Master Block, Block Descriptor segments (each defines a file stored in a corresponding Data Block) and Data Blocks.

Currently fsman can create/delete a file system, import/export files from a file system, list files and display a map of empty/reserved space. For full manual, run "fsman --help".

TODO: 
      
      - Windows version
      - CMake instead of a singular makefile
