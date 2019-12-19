GCC = gcc
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror -g

all : server

server : server.c
	$(GCC) $(FLAGS) -o server server.c

clean :
	rm -f server
