#include <conio.h>
#include "tnd232.h"

int tndlpt_output(char cmd_byte)
{
  unsigned short port = tndlpt_port;
  outp(port, cmd_byte);
  //outp(0x3F8, cmd_byte);
  return 0;
}
