#include "console.h"
//#include <syscall.h>//#include "syscalls.h"
#include <stdio.h>
#include <timer.h>
#include <memory.h>
#include <stdint.h>
#include <interrupt.h>
#include <stddef.h>
//-----------------------------------------------------------------
// init: Early init - run before main()
//-----------------------------------------------------------------


extern void nulluser();
extern void main(int argc, char const *argv[]);
void start(void)
{

 
    // Initialise the UART
    // Baud rate maybe ignored if the baudrate is fixed in HW
    console_init();
    console_putstr("Booting...\n");
    
    heap_init((void *)&_end);
    uint32_t f=heap_free();
    kprintf("maxaddr: %x\n",MAXADDR);
    kprintf("free memory: %d\n",f);
    kprintf("%10d bytes of Xinu code.\n",
    (uint32)&_etext - (uint32)&_text);
    kprintf("           [0x%08X to 0x%08X]\n",
    (uint32)&_text, (uint32)&_etext - 1);
    kprintf("%10d bytes of data.\n",
    (uint32)&_ebss - (uint32)&_sdata);
    kprintf("           [0x%08X to 0x%08X]\n\n",
    (uint32)&_sdata, (uint32)&_ebss - 1);
    
    
    disable();
    
    timer_init();
 
    nulluser();
  // main(0,NULL);
}

