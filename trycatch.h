#ifndef TRYCATCH_H
#define TRYCATCH_H 1

#include <setjmp.h>
#include <stdbool.h>

enum exception {
    EX_OVERFLOW =  0xA0,
    EX_RANGE =     0xA1,
    EX_EOF =       0xA2,
    EX_NULLPTR =   0xA3,
    EX_MEMORY =    0xA4,
    EX_FILE =      0xA5,

    EX_SIGFPE =    0xB0,
    EX_SIGILL =    0xB1,
    EX_SIGSEGV =   0xB2,
    EX_SIGABRT =   0xB3,
    EX_SIGINT =    0xB4,
    EX_SIGTERM =   0xB5
};

#define SIG_MAX 0x20

extern jmp_buf _catch_jmp_buf;
extern jmp_buf _retry_jmp_buf;
extern int _catch_value;
extern volatile bool _retry_attempt;

void init_trycatch(void);
void _catch_sig_handle(int);

#define try                                    \
_catch_value = setjmp(_catch_jmp_buf);         \
setjmp(_retry_jmp_buf);                        \
if (_retry_attempt) {                          \
    _catch_value = 0;                          \
    _retry_attempt = false;                    \
}                                              \
if (!_catch_value)

#define catch_all                              \
else

#define catch(X)                               \
else if (_catch_value == X)

#define finally                                \
if (_catch_value == 0)

#define throw(X)                               \
longjmp(_catch_jmp_buf, X)

#define retry()                                \
_retry_attempt = true;                         \
longjmp(_retry_jmp_buf, 1);                    \

#endif // TRYCATCH_H
