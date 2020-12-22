#include <signal.h>
#include "trycatch.h"

jmp_buf _catch_jmp_buf = { 0 };
jmp_buf _retry_jmp_buf = { 0 };
int _catch_value = 0;
volatile bool _retry_attempt = false;
volatile bool _init_signals = false;
volatile bool _throw_enabled = true;

volatile bool _trextrigger = false;
volatile const char *_trexfile = "";
volatile const char *_trexfunc = "";
volatile unsigned long _trexline = 0;
volatile int _trex = 0;

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
    if (!_throw_enabled) {
        signal(sig, SIG_DFL);
        return;
    }

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
