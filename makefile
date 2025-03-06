output: main.o chip8.o
	g++ main.o chip8.o -o output

main.o: main.cpp
	g++ -c main.cpp

chip8.o: chip8.cpp
	g++ -c chip8.cpp

clean: 
	rm *.o output
