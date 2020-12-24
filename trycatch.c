#include <signal.h>
#include "trycatch.h"

_Thread_local jmp_buf _catch_jmp_buf = { 0 };
_Thread_local int _catch_value = 0;
_Thread_local volatile bool _retry_attempt = false;
_Thread_local volatile bool _init_signals = false;

_Thread_local volatile bool _trextrigger = false;
_Thread_local volatile const char *_trexfile = "";
_Thread_local volatile const char *_trexfunc = "";
_Thread_local volatile unsigned long _trexline = 0;
_Thread_local volatile int _trex = 0;

const char *_exception_string[] = {
    "EX_OVERFLOW",
    "EX_RANGE",
    "EX_EOF",
    "EX_NULLPTR",
    "EX_MEMORY",
    "EX_FILE",

    "EX_SIGFPE",
    "EX_SIGILL",
    "EX_SIGSEGV",
    "EX_SIGABRT",
    "EX_SIGINT",
    "EX_SIGTERM"
};

void _catch_sig_handle(int sig) {
    switch (sig) {
    case SIGFPE:
        throw(EX_SIGFPE);
    case SIGILL:
        throw(EX_SIGILL);
    case SIGSEGV:
        throw(EX_SIGSEGV);
    case SIGABRT:
        throw(EX_SIGABRT);
    case SIGINT:
        throw(EX_SIGINT);
    case SIGTERM:
        throw(EX_SIGTERM);
    #ifdef SIGCONT
    case SIGCONT:
        return;
    #endif
    default:
        signal(sig, SIG_DFL);
        return;
    }
}
