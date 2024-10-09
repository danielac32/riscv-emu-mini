#include "timer.h"
#include "csr.h"
#include <stdint.h>
#include "console.h"
#include <kernel.h>
#include <interrupt.h>
//#include <os.h>
volatile uint64_t *mtimecmp = (uint64_t *)(CLINT_BASE_ADDR + CLINT_MTIMECMP_OFFSET);
volatile uint64_t *mtime = (uint64_t *)(CLINT_BASE_ADDR + CLINT_MTIME_OFFSET);



void init_timer(uint64_t interval){
    uint64_t current_time = *mtime;
    *mtimecmp = current_time + interval;
} 

void clkupdate(uint64_t cycles)
{
    *mtimecmp = (*mtime) + cycles;
}

//-----------------------------------------------------------------
// cpu_timer_get_count:
//-----------------------------------------------------------------
static inline unsigned long cpu_timer_get_count(void)
{    
    unsigned long value;
    asm volatile(".option arch, +zicsr"); 
 
    asm volatile ("csrr %0, time" : "=r" (value) : );
    //value = timel;
    return value;
}


struct irq_context *irq_handler(struct irq_context *ctx){
unsigned int cause_code = ctx->cause & 0xfff;
    switch (cause_code){
        case 3:
            //kprintf("software interruption!\n");
           // ctx->pc+=4;
            //ctx->pc =(uint32)&resched;
        break;
        case 7:
           // kprintf("timer!\n");
            clkhandler();
           // ctx->pc =(uint32)&clkhandler;
        break;
        case 11:
            kprintf("external interruption!\n");
        break;
        default:
            kprintf("unknown async exception!\n");
        break;
    }
return ctx;
}
//--------------------------------------------------------------------------
// timer_init:
//--------------------------------------------------------------------------
void timer_init(void)
{

   clear_interrupts();
   enable_interrupt_status();
   enable_interrupt(7);
   enable_interrupt(3);
   enable_interrupt(11);
   //timel=10;
   init_timer(10);
   exception_set_irq_handler(irq_handler);
}
//--------------------------------------------------------------------------
// timer_sleep:
//--------------------------------------------------------------------------
void timer_sleep(int timeMs)
{
    t_time t = timer_now();

    while (timer_diff(timer_now(), t) < timeMs)
        ;
}
//--------------------------------------------------------------------------
// timer_now:
//--------------------------------------------------------------------------
t_time timer_now(void)
{
    return cpu_timer_get_count();// / (CONFIG_CPU_HZ / 1000);
}
//--------------------------------------------------------------------------
// timer_set_mtimecmp: Non-std mtimecmp support
//--------------------------------------------------------------------------
void timer_set_mtimecmp(t_time next)
{
    csr_write(0x7c0, next);
}
//--------------------------------------------------------------------------
// timer_get_mtime:
//--------------------------------------------------------------------------
t_time timer_get_mtime(void)
{
    return csr_read(0xc00);
}