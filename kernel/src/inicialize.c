#include <os.h>
#include <csr.h>
#include <syscall.h>
//#include <littlefs.h>

 

static	void sysinit(); 	/* Internal system initialization	*/
 
struct  procent proctab[NPROC]; /* Process table            */
struct  sentry  semtab[NSEM];
struct  memblk  memlist;
int prcount;        /* Total number of live processes   */
pid32   currpid;        /* ID of currently executing process    */

/*
void ilde3(){

    while(1){
    	printf("ilde\n");
       sleepms(10);
    }
}*/

void ilde2(){

    while(1){
    	printf("		ilde2\n");
        //sleepms(20);
    }
}

int nullprocess(void) {
printf("init process\n");
//resume(create(ilde, 2048 ,3, "ilde", 0));
resume(create(ilde2, 2048, 1, "ilde2", 0));
//resume(create(shell, 2048, 1, "shell", 0));

return 0;
}

 
void	nulluser(){
uint32	free_mem;	
kprintf ("Build date: %s %s\n\n", __DATE__, __TIME__);
disable();
sysinit();
preempt = QUANTUM;
 

int pid=create(NULL, 512, 1, "ilde", 0);
struct procent * prptr = &proctab[pid];
prptr->prstate = PR_CURR;
//ready(create(ilde, 2048, 3, "ilde", 0));
enable();
ready(create(nullprocess, 2048, 3, "start process", 0));
//nullprocess();
for(;;){
//	kprintf("riscv32 ......... \n");
//	MDELAY(1000);
}
}


static	void	sysinit()
{
	int32	i;
	struct procent *prptr;     /* Ptr to process table entry   */
    struct  sentry  *semptr;
    clkinit();
    syscall_os_init();
	for (int i = 0; i < 2; ++i)
	{
		kprintf("\n");
	}
	
	prcount = 0;
//	prcount = 1;


	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->elf = FALSE;
        prptr->prprio = 0;
	}


    for (i = 0; i < NSEM; i++) {
        semptr = &semtab[i];
        semptr->sstate = S_FREE;
        semptr->scount = 0;
        semptr->squeue = newqueue();
    }

    
    readylist = newqueue();

	
	for (i = 0; i < NDEVS; i++) {
		init(i);
	}
	return;
}
