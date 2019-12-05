#include <stdio.h>
#include <ctype.h>


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

void invert_case() {
	for (int i = 0; i < 27; ++i) {
		alphabet[i] = (islower(alphabet[i])) 
			? toupper(alphabet[i])
			: tolower(alphabet[i]);
	}
}
