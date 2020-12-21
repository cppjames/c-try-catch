#include <stdio.h>
#include <tgmath.h>
#include "trycatch.h"

void print_sqrt(float x) {
	if (x < 0)
		throw(EX_RANGE);
	int out = printf("As follows: %g\n", sqrt(x));
	if (out == -1)
		throw(EX_EOF);
}

int main() {
	
	try {
		float f = 0;
		scanf("%f", &f);
		print_sqrt(f);
	} catch(EX_RANGE) {
		printf("We got a range problem.\n");
		retry();
	} catch (EX_EOF) {
		printf("Something's wrong with the streams.\n");
		retry();
	} catch_all() {
		printf("Something's wrong but aren't sure what.\n");
		return 1;
	}

	return 0;
}
