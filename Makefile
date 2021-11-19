CC = g++
CFLAGS = -Wall -O3

all: analyzer

analyzer: analyzer.o subject.o input.o
	$(CC) $(CFLAGS) -o analyzer *.o

analyzer.o: analyzer.cpp subject.hpp
	$(CC) $(CFLAGS) -c analyzer.cpp

input.o: input.cpp input.hpp
	$(CC) $(CFLAGS) -c input.cpp

subject.o: subject.cpp subject.hpp
	$(CC) $(CFLAGS) -c subject.cpp

clean:
	rm -f *.o analyzer
