#include "console.h"
//#include <syscall.h>//#include "syscalls.h"
#include <stdio.h>
#include <timer.h>
#include <memory.h>
#include <stdint.h>
#include <interrupt.h>
//-----------------------------------------------------------------
// init: Early init - run before main()
//-----------------------------------------------------------------


extern void nulluser();
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
    
    //kprintf("enable IRQs\n");
    // Initialise the newlib syscall interface
    //syscall_init();
    //csr_set_irq_enable();
    //kprintf("disable IRQs\n");
    disable();
    //kprintf("init timer\n");
    timer_init();
    //kprintf("waiting...\n");
    nulluser();
    //soft_trap();
}

