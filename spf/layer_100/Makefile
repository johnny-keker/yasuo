GCC = gcc
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror

all : tail

tail : keker_tail.c
	$(GCC) $(FLAGS) -o keker_tail keker_tail.c

clean :
	rm keker_tail
