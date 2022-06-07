# -*- MakeFile -*-

# @Hosein Hadipour
# Jun 07, 2022

# The main structure of a make file commands is as follows: 
# target: dependencies
#       action
# Example: 
# output: main.o minor1.o minor2.o
#         g++ -o main main.o minor1.o minor2.o
# Some compiler flags:
# -E                       Preprocess only; do not compile, assemble or link.
# -S                       Compile only; do not assemble or link. (generates an assembly file and stop)
# -c                       Compile and assemble, but do not link. (generates an assembly code and converts it to the machine code by assembler)
# -o <file>                Place the output into <file>. (Links assembled files, and builds an executable file)
# -g    adds debugging information to the executable file
# -Wall turns on most, but not all, compiler warnings
# If you don't use a flag at all, your compiler (gcc here) do the process, compile

# compiler
# define the C compiler to use
# for C++ define  CC = g++
CC = gcc
CFLAGS = -g -Wall -O3
# the build target(s) executable:
TARGET = test
# If you execute make without a flag, it does the actions under the "all" target by default
all: aes.o test.o
	$(CC) $(CFLAGS) -o $(TARGET) aes.o test.o
test.o: aes.o
	$(CC) $(CFLAGS) -c test.c
aes.o: aes.h
	$(CC) $(CFLAGS) -c aes.c aes.h
clean:
	rm -f *.o *gch $(TARGET)