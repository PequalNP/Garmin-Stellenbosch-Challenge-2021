output: main.o
	g++ main.o -o output
main.o: main.cpp wav.h
	g++ -c main.cpp
clean:
	rm *.o  output
