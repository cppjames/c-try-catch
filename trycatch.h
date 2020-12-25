#ifndef TRYCATCH_H
#define TRYCATCH_H 1

#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef TRACE_THROW
    #include <stdio.h>
#endif

#define EX_BEGIN  0xA0
#define SIG_BEGIN 0xA6

enum exception {
    EX_OVERFLOW =  0xA0,
    EX_RANGE =     0xA1,
    EX_EOF =       0xA2,
    EX_NULLPTR =   0xA3,
    EX_MEMORY =    0xA4,
    EX_FILE =      0xA5,

    EX_SIGFPE =    0xA6,
    EX_SIGILL =    0xA7,
    EX_SIGSEGV =   0xA8,
    EX_SIGABRT =   0xA9,
    EX_SIGINT =    0xAA,
    EX_SIGTERM =   0xAB
};

extern const char *_exception_string[];

#define SIG_MAX 0x20

extern _Thread_local jmp_buf _catch_jmp_buf;
extern _Thread_local int _catch_value;
extern _Thread_local volatile bool _retry_attempt;
extern _Thread_local volatile bool _init_signals;

extern _Thread_local volatile bool _trextrigger;
extern _Thread_local volatile const char *_trexfile;
extern _Thread_local volatile const char *_trexfunc;
extern _Thread_local volatile unsigned long _trexline;
extern _Thread_local volatile int _trex;

void _catch_sig_handle(int);

#define _do_nothing()                            \
    do { /* nothing */ } while (false)

#define _trace_prepare(X)                        \
    do { _trextrigger = true;                    \
    _trexfile = __FILE__;                        \
    _trexfunc = __func__;                        \
    _trexline = __LINE__+0UL;                    \
    _trex = X; } while (false)

#ifdef TRACE_THROW
    #define _trace_throw(X)                          \
        fprintf(stderr,                              \
                "[%s] %s:%lu: Exception %s.\n",      \
                _trexfile, _trexfunc, _trexline,     \
                _exception_string[_trex - EX_BEGIN])

    #define _trace_signal()                          \
        fprintf(stderr,                              \
                "Interrupted by signal: %s.\n",      \
                _exception_string[_trex - EX_BEGIN])

    #define _trace_if_triggered()                    \
        do { if (_trextrigger)                       \
                 if (_trex >= SIG_BEGIN)             \
                     _trace_signal();                \
                 else                                \
                     _trace_throw();                 \
             _trextrigger = false;                   \
        } while (false)
        
#else
    #define _trace_if_triggered() _do_nothing()
#endif

#ifndef NO_THROW
    #define _bind_signals()                        \
        do {                                       \
            for (uint8_t i = 1; i < SIG_MAX; i++)  \
                signal(i, _catch_sig_handle);      \
            _init_signals = true;                  \
        } while (false)
#else
    #define _bind_signals() _do_nothing()
#endif

#define try                                        \
    if (!_init_signals)                            \
        _bind_signals();                           \
    _catch_value = setjmp(_catch_jmp_buf);         \
    _trace_if_triggered();                         \
    if (_retry_attempt) {                          \
        _catch_value = 0;                          \
        _retry_attempt = false;                    \
    }                                              \
    if (!_catch_value)

#define finally                                    \
    if (_catch_value == 0)

#ifndef NO_THROW
    #define catch(X)                                 \
        else if (_catch_value == X)

    #define throw(X)                                 \
        do { _trace_prepare(X);                      \
        longjmp(_catch_jmp_buf, X); } while (false)

    #define catch_all                                \
        else
#else
    #define catch(X) else if (false)
    #define catch_all else if (false)
    #define throw(X) _do_nothing()
#endif

#ifndef NO_RETRY
    #define retry()                                \
        do { _retry_attempt = true;                \
        longjmp(_catch_jmp_buf, 1); } while (false)
#else
    #define retry() _do_nothing()
#endif

#endif // TRYCATCH_H
