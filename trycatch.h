#ifndef TRYCATCH_H
#define TRYCATCH_H 1

#include <setjmp.h>
#include <stdbool.h>

enum exception {
    EX_OVERFLOW =  0xF0,
    EX_RANGE =     0xF1,
    EX_EOF =       0xF2,
    EX_NULLPTR =   0xF3,
    EX_MEMORY =    0xF4,
    EX_FILE =      0xF5
};

extern jmp_buf _catch_jmp_buf;
extern jmp_buf _retry_jmp_buf;
extern int _catch_value;
extern volatile bool _retry_attempt;

#define try                                    \
_catch_value = setjmp(_catch_jmp_buf);         \
setjmp(_retry_jmp_buf);                        \
if (_retry_attempt) {                          \
    _catch_value = 0;                      \
    _retry_attempt = false;                \
}                                              \
if (!_catch_value)

#define catch_all()                            \
else

#define catch(X)                               \
else if (_catch_value == X)

#define throw(X)                               \
longjmp(_catch_jmp_buf, X)

#define retry()                                \
_retry_attempt = true;                         \
longjmp(_retry_jmp_buf, 1);                    \

#endif // TRYCATCH_H
