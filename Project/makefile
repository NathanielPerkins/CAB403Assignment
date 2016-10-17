# Compiler Command
CC=gcc
# Compiler flags: Warnings, errors, compiler directives
CFLAGS = -std=gnu99 -g -Wall -Werror
#get all files with .c extensions
LFLAGS = -lpthread
SERVER_SRCS = Server.c fileControl.c
CLIENT_SRCS = Client.c
#pattern substitution all 'filename.c' gets replaced with 'filename' in $(SRCS)
all: server client
server: $(SERVER_SRCS)
	$(CC) $(CFLAGS) $(LFLAGS) $(SERVER_SRCS) -o Server
client: $(CLIENT_SRCS)
	$(CC) $(CFLAGS) $(CLIENT_SRCS) -o Client
clean:
	rm -f Server Client
rebuild: clean all
memory:
	valgrind --leak-check=yes ./Server
