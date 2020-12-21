#include "trycatch.h"

jmp_buf _catch_jmp_buf = { 0 };
jmp_buf _retry_jmp_buf = { 0 };
int _catch_value = 0;
volatile bool _retry_attempt = false;
