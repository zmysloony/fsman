bold=$(tput bold)
normal=$(tput sgr0)
echo "${bold}1. First, we create a 50kB, empty file system.${normal}"
./fsman -c 50000 -f 50test
./fsman -m -f 50test
echo
echo "${bold}2. Unfortunately due to static (4096B) block size, we can only allocate 11 files in such a system.${normal}"
./fsman -a wtf1 -f 50test
./fsman -a wtf2 -f 50test
./fsman -a wtf3 -f 50test
./fsman -a wtf4 -f 50test
./fsman -a wtf5 -f 50test
./fsman -a wtf6 -f 50test
./fsman -a wtf7 -f 50test
./fsman -a wtf8 -f 50test
./fsman -a wtf9 -f 50test
./fsman -a wtf10 -f 50test
./fsman -a wtf11 -f 50test
./fsman -m -f 50test
echo
echo "${bold}3. But we have no external fragmentation this way!${normal}"
echo "${bold}  a) Let's make a larger fs and add a larger file to it.${normal}"
./fsman -c 5000000 -f 5000test
./fsman -a 50test -f 5000test
./fsman -m -f 5000test
echo
echo "${bold}  b) Now we add a some small files and remove the large one.${normal}"
./fsman -a wtf1 -f 5000test
./fsman -a wtf2 -f 5000test
./fsman -a wtf3 -f 5000test
./fsman -a wtf4 -f 5000test
./fsman -r 50test -f 5000test
./fsman -m -f 5000test
echo
echo "${bold}  c) And we can see as, after adding some files, they land in the first free block.${normal}"
./fsman -a wtf5 -f 5000test
./fsman -a wtf6 -f 5000test
./fsman -m -f 5000test
echo
echo "${bold}  d) Now after adding the large file back again, file system has no problem filling the gaps.${normal}"
./fsman -a 50test -f 5000test;
./fsman -m -f 5000test
./fsman -d -f 5000test
./fsman -d -f 50test
