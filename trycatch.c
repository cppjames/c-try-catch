#include <signal.h>
#include "trycatch.h"

jmp_buf _catch_jmp_buf = { 0 };
jmp_buf _retry_jmp_buf = { 0 };
int _catch_value = 0;
volatile bool _retry_attempt = false;

void init_trycatch(void) {
    for (uint8_t i = 1; i < SIG_MAX; i++)
        signal(i, _catch_sig_handle);
}

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
