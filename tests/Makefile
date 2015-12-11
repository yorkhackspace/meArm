CC=gcc
CPP=g++
CFLAGS=-I..

%.o: %.cpp
	$(CPP) -o $@ -c $< $(CFLAGS)

test_ik: test_ik.o ../ik.o
	gcc -o $@ $^ $(CFLAGS) -lm
