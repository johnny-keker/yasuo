GCC = gcc
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror -g

all : client server

client : client.c
	$(GCC) $(FLAGS) -o client client.c

server : server.c
	$(GCC) $(FLAGS) -o server server.c

clean :
	rm client server
