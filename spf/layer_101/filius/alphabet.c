#include <stdio.h>


char alphabet[26] = {
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
 	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

void print_alphabet() {
	char* alpha = alphabet;
	while(*alpha) {
		printf("%c ", *alpha);
		alpha++;
	}
}
