CC = g++
CFLAGS = -g -Wall --std=c++11

ROOT = ../..

ENV_DIR = $(ROOT)/environments

all: asg1
asg1:
	$(CC) $(CFLAGS) $(ENV_DIR)/*.cpp Driver.cpp  -o bin/asg1

test: asg1test
asg1test:
	$(CC) $(CFLAGS) GridBasedMap.cpp NAryTree.cpp Test.cpp  -o bin/asg1test
clean:
	rm bin/asg1test
	rm bin/asg1