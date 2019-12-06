GCC = gcc
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror -g

all : pater filius

pater : pater_client pater_server

filius : filius_mut filius_rw

pater_client : pater/client.c
	$(GCC) $(FLAGS) -o pater_client pater/client.c

pater_server : pater/server.c
	$(GCC) $(FLAGS) -o pater_server pater/server.c

filius_mut : filius/filius.c filius/alphabet.c
	$(GCC) $(FLAGS) -D MUTEX -pthread -o filius_mut filius/alphabet.c filius/filius.c

filius_rw : filius/filius.c filius/alphabet.c
	$(GCC) $(FLAGS) -D RW -pthread -o filius_rw filius/alphabet.c filius/filius.c

clean :
	rm pater_client pater_server
