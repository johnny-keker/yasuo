GCC = gcc
# add -lsocket on helios
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror -g
FLAGS_NON_PEDANTIC = -Wall -Wextra --std=gnu99 -g

all : server client

server : server.c logger.c error.c
	$(GCC) $(FLAGS) -o server error.c logger.c server.c

client : client.c logger.c error.c
	$(GCC) $(FLAGS) -o client error.c logger.c client.c

server_non_pedantic : server.c logger.c error.c
	$(GCC) $(FLAGS_NON_PEDANTIC) -o server error.c logger.c server.c

logger : logger.c
	$(GCC) $(FLAGS) -o logger logger.c

clean :
	rm -f server
