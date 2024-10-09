#include "console.h"
#include <stdio.h>
#include <timer.h>
#include <stdarg.h>

extern	void	_doprnt(char *, va_list, int (*)(int));


//-----------------------------------------------------------------
// Console stubs
//-----------------------------------------------------------------
void console_init() { }
void console_putchar(int ch) { 
	asm volatile(".option arch, +zicsr");
	asm volatile("csrw 0x402, %0"
               :
               : "r"(ch));
}
int  console_getchar(void) { return -1; }
void console_putstr(const char *s)
{
	while (*s)
		console_putchar(*s++);
}
int kputc(int ch){
 asm volatile(".option arch, +zicsr");
 asm volatile("csrw 0x402, %0"
               :
               : "r"(ch));
 return 0;
}
void kprintf(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
   _doprnt(fmt, ap, (int (*)(int))kputc);
    va_end(ap);
}
