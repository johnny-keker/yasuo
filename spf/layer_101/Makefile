GCC = gcc
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror -g

all : pater_client pater_server filius

pater : pater_client pater_server

filius : filius_c

pater_client : pater/client.c
	$(GCC) $(FLAGS) -o pater_client pater/client.c

pater_server : pater/server.c
	$(GCC) $(FLAGS) -o pater_server pater/server.c

filius_c : filius/filius.c filius/alphabet.c
	$(GCC) $(FLAGS) -o filius_run filius/alphabet.c filius/filius.c

clean :
	rm pater_client pater_server
