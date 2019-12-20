GCC = gcc
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror -g
FLAGS_NON_PEDANTIC = -Wall -Wextra --std=gnu99 -g

all : server

server : server.c
	$(GCC) $(FLAGS) -o server server.c

server_non_pedantic : server.c
	$(GCC) $(FLAGS_NON_PEDANTIC) -o server server.c

clean :
	rm -f server
