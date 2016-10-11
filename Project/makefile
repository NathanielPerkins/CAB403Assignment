# Compiler Command
CC=gcc
# Compiler flags: Warnings, errors, compiler directives
CFLAGS = -std=gnu99 -g -Wall -Werror
#get all files with .c extensions
LFLAGS = -lpthread
SRCS = $(wildcard *.c)
#pattern substitution all 'filename.c' gets replaced with 'filename' in $(SRCS)
PROG = CAB403_UrviP_NathanP
program: $(SRCS)
	$(CC) $(CFLAGS) $(LFLAGS) $(SRCS) -o $(PROG)
clean:
	rm -f $(PROG)
rebuild: clean all
memory:
	valgrind --leak-check=yes ./$(PROG)
