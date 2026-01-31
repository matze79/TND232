#include <conio.h>
#include <i86.h>
#include "tnd232.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "resident.h"
#include "cputype.h"
#include "tndinit.h"

void init_com(unsigned short base) {
    unsigned char lcr;
    // Baudrate 115200 setzen
    // Baudrate = 115200 -> Divisor = 115200 / 115200 = 1
    outp(base + 1, 0x00);    // Interrupts aus
    outp(base + 3, 0x80);    // DLAB = 1 (Zugriff auf Divisor)
    outp(base + 0, 0x01);    // Divisor Low Byte
    outp(base + 1, 0x00);    // Divisor High Byte
    outp(base + 3, 0x03);    // 8 Bits, keine Parität, 1 Stopbit
    outp(base + 2, 0xC7);    // FIFO aktivieren, leeren, 14 Byte Schwelle
    outp(base + 4, 0x0B);    // IRQs aktivieren, RTS/DSR setzen
}

int tndlpt_init(bool force)
{
  unsigned short port = tndlpt_port;
  init_com(port);  

  /* Initialize the sound chip */
  if (!tndlpt_output(0x9F) && !force) {
    // return 0; // Return 0 - tndlpt not detected
    // ADD RS232 Device detecting code here
     return 1;
  }

  return 1;
}
