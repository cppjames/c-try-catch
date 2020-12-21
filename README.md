# Try-Catch blocks for C

Small experiment that simulates try-catch blocks in C using `setjmp`/`longjmp`. Please note that it doesn't support nested try blocks and it is not thread safe.

Usage:

```c
#include <stdio.h>
#include <tgmath.h>
#include "trycatch.h"

float ex_sqrt(float x) {
    if (x < 0)
        throw(EX_RANGE);
    return sqrt(x);
}

int main() {
    init_trycatch();

    try {
        printf("%f", ex_sqrt(5)); // The function may throw.
    } catch(EX_RANGE) {
        printf("Input error. Trying again...");
        // This will try again until the function doesn't
        // overflow anymore (not recommended unless you are sure
        // this can happen).
        retry();
    } catch(EX_SIGINT) {
        printf("We have been interrupted. I repeat: we have been interrupted.");
    }

    return 0;
}
```

A slightly more complex example can be found in `test.c`.
