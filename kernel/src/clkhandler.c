//#include <stdint.h>
//#include <kernel.h>
//#include <interrupt.h>
//#include <string.h>
 
#include <os.h>
#include <csr.h>
#include <time.h>

 


//extern void mark_context_switch_needed();

void clkhandler(void){
         
        //clkupdate(100);//100
        clkupdate(100);
        count1000++;
        //printf("timer\n");
        /* After 1 sec, increment clktime */

        if(count1000 >= 1000) {
            clktime++;
            count1000 = 0;
            
        }

        /* check if sleep queue is empty */

            if(!isempty(sleepq)) {
                /* sleepq nonempty, decrement the key of */
                /* topmost process on sleepq         */

                if((--queuetab[firstid(sleepq)].qkey) == 0) {

                    wakeup();
                }
            }

            /* Decrement the preemption counter */
            /* Reschedule if necessary      */

           if((--preempt) == 0) {
                preempt = QUANTUM;
                // PendSV call
                yield();

               // mark_context_switch_needed();
           }
        
}
