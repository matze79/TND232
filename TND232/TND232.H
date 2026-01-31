#include <stdbool.h>

extern unsigned short tndlpt_port;

int tndlpt_init(bool force);

int tndlpt_output(char cmd_byte);

#ifdef _M_I86
#pragma aux tndlpt_output parm [ax] value [ax] modify exact [ax]
#else
#pragma aux tndlpt_output parm [eax] value [eax] modify exact [eax]
#endif
