GCC = gcc
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror -g

all : tail xargs

tail : keker_tail.c
	$(GCC) $(FLAGS) -o keker_tail keker_tail.c

xargs : keker_xargs.c
	$(GCC) $(FLAGS) -o keker_xargs keker_xargs.c

clean :
	rm keker_tail keker_xargs
