#include <stdio.h>
#include <tgmath.h>
#include "trycatch.h"

void print_sqrt(float x) {
    if (x < 0)
        throw(EX_RANGE);
    int out = printf("As follows: %g\n", sqrt(x));
    if (out < 0)
        throw(EX_EOF);
}

int main() {
    // Example for throw().
    try {
        printf("Get square root of: ");
        float f = 0;
        scanf("%f", &f);
        print_sqrt(f);
    } catch(EX_RANGE) {
        printf("We got a range problem.\n");
        retry();
    } catch(EX_EOF) {
        printf("Something's wrong with the streams.\n");
        retry();
    } catch_all {
        printf("Something's wrong but aren't sure what.\n");
        return 1;
    } finally {
        printf("You got it right.\n");
    }

    // Example for signal handlers.
    try {
        for (size_t i = 0; i < 1000000000; i++)
            ;
        printf("Divide 10 by: ");
        int x = 0;
        scanf("%d", &x);
        printf("Result is %d.\n", 10/x);

        printf("Enter an address: ");
        size_t p = 0;
        scanf("%u", &p);
        printf("Let's see what's there: %c\n", *((unsigned char*)p));
    } catch(EX_SIGINT) {
        printf("Is somebody trying to kill us?\n");
    } catch(EX_SIGFPE) {
        printf("Good job: you broke the calculator.\n");
    } catch(EX_SIGSEGV) {
        printf("You can't look there.\n");
    }

    return 0;
}
