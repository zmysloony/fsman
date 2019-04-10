default: main
options.o: options.cpp options.h
	g++ -g -c options.cpp options.h
manage_fs.o: manage_fs.cpp manage_fs.h
	g++ -g -c manage_fs.cpp manage_fs.h
manage_blocks.o: manage_blocks.cpp manage_blocks.h
	g++ -g -c manage_blocks.cpp manage_blocks.h
main: manage_fs.o manage_blocks.o options.o main.cpp
	g++ -g -o fsman manage_fs.o manage_blocks.o options.o main.cpp
clean:
	rm *.o
