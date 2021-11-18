CC = g++
CFLAGS = -Wall -O3

all: analyzer

analyzer: analyzer.o subject.o
	$(CC) $(CFLAGS) -o analyzer *.o

analyzer.o: analyzer.cpp subject.hpp
	$(CC) $(CFLAGS) -c analyzer.cpp

subject.o: subject.cpp subject.hpp
	$(CC) $(CFLAGS) -c subject.cpp

clean:
	rm -f *.o analyzer
