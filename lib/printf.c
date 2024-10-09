/* printf.c - printf*/


#if 1
#include <os.h>
//#include <stdio.h>
#include <syscall.h>

 
extern void _fdoprnt(char *, va_list, int (*)(did32, char), int);

 
/*------------------------------------------------------------------------
 *  printf  -  standard C printf function
 *------------------------------------------------------------------------
 */



extern  void    _doprnt(char *, va_list, int (*)(int));


int printf(
      const char        *fmt,
      ...
    )
{
    va_list ap;

    va_start(ap, fmt);
    _fdoprnt((char *)fmt, ap, putc, CONSOLE);
    //_doprnt(fmt, ap, (int (*)(int))kputc);
    va_end(ap);

    return 0;
}


void hexDump2(unsigned int offset, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", offset+i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}


#endif