GCC = gcc
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror -g

all : pater filius spiritus_sanctus

pater : pater_client pater_server

filius : filius_mut filius_rw filius_sem_pos filius_sem_sysv

spiritus_sanctus : spiritus_sanctus_server spiritus_sanctus_client

pater_client : pater/client.c
	$(GCC) $(FLAGS) -o pater_client pater/client.c

pater_server : pater/server.c
	$(GCC) $(FLAGS) -o pater_server pater/server.c

filius_mut : filius/filius.c filius/alphabet.c
	$(GCC) $(FLAGS) -D MUTEX -pthread -o filius_mut filius/alphabet.c filius/filius.c

filius_rw : filius/filius.c filius/alphabet.c
	$(GCC) $(FLAGS) -D RW -pthread -o filius_rw filius/alphabet.c filius/filius.c

filius_sem_pos : filius/filius.c filius/alphabet.c
	$(GCC) $(FLAGS) -D SEM_POS -pthread -o filius_sem_pos filius/alphabet.c filius/filius.c
	
filius_sem_sysv : filius/filius.c filius/alphabet.c
	$(GCC) $(FLAGS) -D SEM_SYSV -pthread -o filius_sem_sysv filius/alphabet.c filius/filius.c

spiritus_sanctus_server : spiritus_sanctus/server.c
	$(GCC) $(FLAGS) -D SEM_SYSV -pthread -o spiritus_sanctus_srv spiritus_sanctus/server.c

spiritus_sanctus_client : spiritus_sanctus/client.c
	$(GCC) $(FLAGS) -D SEM_SYSV -pthread -o spiritus_sanctus_clt spiritus_sanctus/client.c

clean :
	rm -f pater_client pater_server filius_mut filius_rw filius_sem_pos filius_sem_sysv spiritus_sanctus_srv spiritus_sanctus_clt
