CC = g++
CFLAGS = -g -Wall --std=c++11

all: asg1
asg1:
	mkdir -p bin
	$(CC) $(CFLAGS) GridBasedMap.cpp NAryTree.cpp Driver.cpp  -o bin/asg1

test: asg1test
asg1test:
	mkdir -p bin
	$(CC) $(CFLAGS) GridBasedMap.cpp NAryTree.cpp Test.cpp  -o bin/asg1test
clean:
	rm -rf bin