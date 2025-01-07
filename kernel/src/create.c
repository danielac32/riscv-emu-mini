/* create.c - create, newpid */

#include <os.h>
#include <syscall.h>
#include <csr.h>
local	int newpid();

#define	roundew(x)	( (x+3)& ~0x3)

/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */


extern void *setupStackFromShell(void *stackaddr, void *procaddr, void *retaddr, uint nargs, char *args[]);

extern void *setupStack(void *stackaddr, void *procaddr,
                 void *retaddr, uint nargs, va_list ap);

pid32	createFromShell(
	  void		*procaddr,	/* procedure address		*/
	  uint32	ssize,		/* stack size in bytes		*/
	  pri16		priority,	/* process priority > 0		*/
	  char		*name,		/* name (for debugging)		*/
	  uint32	nargs,		/* number of args that follow	*/
	  char *args[]
	)
{
	intmask 	mask;    	/* interrupt mask		*/
	pid32		pid;		/* stores new process id	*/
	struct	procent	*prptr;		/* pointer to proc. table entry */
	int32		i;
	uint32		*a;		/* points to list of args	*/
	uint32		*saddr;		/* stack address		*/

	mask = disable();
	 
 
    /*uint32 *saddr = malloc(ssize);
    if(saddr==NULL){
    	kprintf("error malloc\n");
    	exit(0);
    }
    memset(saddr, 0, ssize); */
    ssize = (uint32) roundew(ssize);
    if (((saddr = (uint32 *)getstk(ssize)) ==
	    (uint32 *)SYSERR )) {
		kprintf("create error %s\n",name);
		restore(mask);
		return SYSERR;
	}


	prcount++;
	pid=newpid();
	if (SYSERR == (int)pid)
    {
        kprintf("error thrnew %s\n",name);
        free(saddr);
        restore(mask);
        return SYSERR;
    }

	prptr = &proctab[pid];
    prptr->elf=FALSE;
    //prptr->pid=pid;
    prptr->prhasmsg = FALSE;
    prptr->prparent = (pid32)getpid();
	/* initialize process table entry for new process */
	prptr->prstate = PR_SUSP;	/* initial state is suspended	*/
	prptr->prstkbase = saddr;
	prptr->prstklen = ssize;
	prptr->prname[PNMLEN-1] = NULLCH;
	for (i=0 ; i<PNMLEN-1 && (prptr->prname[i]=name[i])!=NULLCH; i++)
		;
	 
	/* set up initial device descriptors for the shell		*/
	prptr->prdesc[0] = CONSOLE;	/* stdin  is CONSOLE device	*/
	prptr->prdesc[1] = CONSOLE;	/* stdout is CONSOLE device	*/
	prptr->prdesc[2] = CONSOLE;	/* stderr is CONSOLE device	*/
	//va_start(ap, nargs);
    //prptr->prstkptr = setupStack(saddr, procaddr, INITRET, nargs, ap);
    //va_end(ap);
   // saddr += ssize;
    prptr->prstkptr = setupStackFromShell(saddr, procaddr, INITRET, nargs, args);
	restore(mask);
	return pid;
}


pid32	create(
	  void		*procaddr,	/* procedure address		*/
	  uint32	ssize,		/* stack size in bytes		*/
	  pri16		priority,	/* process priority > 0		*/
	  char		*name,		/* name (for debugging)		*/
	  uint32	nargs,		/* number of args that follow	*/
	  ...
	)
{
	intmask 	mask;    	/* interrupt mask		*/
	pid32		pid;		/* stores new process id	*/
	struct	procent	*prptr;		/* pointer to proc. table entry */
	uint32		i;
	//uint32		*a;		/* points to list of args	*/
	uint32		*saddr;		/* stack address		*/
    va_list ap;  

	mask = disable();
	 
 
    /*uint32 *saddr = malloc(ssize);
    if(saddr==NULL){
    	kprintf("error malloc\n");
    	exit(0);
    }
    memset(saddr, 0, ssize); */
    ssize = (uint32) roundew(ssize);
    if (((saddr = (uint32 *)getstk(ssize)) ==
	    (uint32 *)SYSERR )) {
		kprintf("create error %s\n",name);
		restore(mask);
		return SYSERR;
	}

	prcount++;
	pid=newpid();
	if (SYSERR == (int)pid)
    {
        kprintf("error thrnew %s\n",name);
        free(saddr);
        restore(mask);
        return SYSERR;
    }

	prptr = &proctab[pid];
    prptr->elf=FALSE;
    //prptr->pid=pid;
    prptr->prhasmsg = FALSE;
    prptr->prparent = (pid32)getpid();
	/* initialize process table entry for new process */
	prptr->prstate = PR_SUSP;	/* initial state is suspended	*/
	prptr->prstkbase = saddr;
	prptr->prstklen = ssize;
	prptr->prname[PNMLEN-1] = NULLCH;
	for (i=0 ; i<PNMLEN-1 && (prptr->prname[i]=name[i])!=NULLCH; i++)
		;
	 
	/* set up initial device descriptors for the shell		*/
	prptr->prdesc[0] = CONSOLE;	/* stdin  is CONSOLE device	*/
	prptr->prdesc[1] = CONSOLE;	/* stdout is CONSOLE device	*/
	prptr->prdesc[2] = CONSOLE;	/* stderr is CONSOLE device	*/
	//va_start(ap, nargs);
    //prptr->prstkptr = setupStack(saddr, procaddr, INITRET, nargs, ap);
    //va_end(ap);
    //saddr += 32;
    va_start(ap, nargs);
    for (int i = 0; i < nargs; ++i)
    {
    	prptr->arg[i]=va_arg(ap, uint32);
    }
    va_end(ap);

    prptr->prstkptr = setupStackFromShell(saddr, procaddr, INITRET, nargs, prptr->arg);
    /*va_start(ap, nargs);
	for (i = 0; i < nargs; i++) {
		kprintf("..>%s\n", (char *)va_arg(ap, uint32));
	}
    va_end(ap);*/

    //va_start(ap, nargs);
    //prptr->prstkptr = setupStack(saddr, procaddr, INITRET, nargs, ap);
    //va_end(ap);
  
	restore(mask);
	return pid;
}

/*------------------------------------------------------------------------
 *  newpid  -  Obtain a new (free) process ID
 *------------------------------------------------------------------------
 */
local	pid32	newpid(void)
{
	uint32	i;			/* iterate through all processes*/
//	static	pid32 nextpid = 1;	/* position in table to try or	*/
	static	pid32 nextpid = 0;	/* position in table to try or	*/
					/*  one beyond end of table	*/

	/* check all NPROC slots */

	for (i = 0; i < NPROC; i++) {
		nextpid %= NPROC;	/* wrap around to beginning */
		if (proctab[nextpid].prstate == PR_FREE) {
			return nextpid++;
		} else {
			nextpid++;
		}
	}
	return (pid32) SYSERR;
}



/*------------------------------------------------------------------------
 *  getpid  -  Return the ID of the currently executing process
 *------------------------------------------------------------------------
 */
pid32	getpid(void)
{
	return (currpid);
}

/*------------------------------------------------------------------------
 *  chprio  -  Change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
pri16	chprio(
	  pid32		pid,		/* ID of process to change	*/
	  pri16		newprio		/* New priority			*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	pri16	oldprio;		/* Priority to return		*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return (pri16) SYSERR;
	}
	prptr = &proctab[pid];
	oldprio = prptr->prprio;
	prptr->prprio = newprio;
	restore(mask);
	return oldprio;
}

/*------------------------------------------------------------------------
 *  getprio  -  Return the scheduling priority of a process
 *------------------------------------------------------------------------
 */
syscall	getprio(
	  pid32		pid		/* Process ID			*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	uint32	prio;			/* Priority to return		*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
	prio = proctab[pid].prprio;
	restore(mask);
	return prio;
}

/*------------------------------------------------------------------------
 *  kill  -  Kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
syscall	kill(
	  pid32		pid		/* ID of process to kill	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	int32	i;			/* Index into descriptors	*/

	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)
	    || ((prptr = &proctab[pid])->prstate) == PR_FREE) {
		restore(mask);
		return SYSERR;
	}

	if (--prcount <= 1) {		/* Last user process completes	*/
		xdone();
	}

	send(prptr->prparent, pid);
	//for (i=0; i<3; i++) {
	//	close(prptr->prdesc[i]);
	//}
	freestk(prptr->prstkbase, prptr->prstklen);
	//free(prptr->prstkbase);
    if(prptr->elf == TRUE){

    	//kprintf("clean space %d\n",prptr->prstklen);
        free(prptr->img);
       // kprintf("offset: %08x\n",prptr->img);
        //freemem((char *)prptr->img,prptr->size);
    }

	switch (prptr->prstate) {
	case PR_CURR:
		prptr->prstate = PR_FREE;	/* Suicide */
		resched();

	case PR_SLEEP:
	case PR_RECTIM:
		unsleep(pid);
		prptr->prstate = PR_FREE;
		break;

	case PR_WAIT:
		semtab[prptr->prsem].scount++;
		/* Fall through */

	case PR_READY:
		getitem(pid);		/* Remove from queue */
		/* Fall through */

	default:
		prptr->prstate = PR_FREE;
	}

	restore(mask);
	return OK;
}


/*------------------------------------------------------------------------
 *  yield  -  Voluntarily relinquish the CPU (end a timeslice)
 *------------------------------------------------------------------------
 */
syscall	yield(void)
{
	intmask	mask;			/* Saved interrupt mask		*/

	//mask = disable();
	//resched();
	//restore(mask);
	//syscallp.yield();
	__syscall(OS_YIELD);
	return OK;
}

/*------------------------------------------------------------------------
 *  wakeup  -  Called by clock interrupt handler to awaken processes
 *------------------------------------------------------------------------
 */
void	wakeup(void)
{
	/* Awaken all processes that have no more time to sleep */
	resched_cntl(DEFER_START);
	while (nonempty(sleepq) && (firstkey(sleepq) <= 0)) {
		ready(dequeue(sleepq));
	}

	resched_cntl(DEFER_STOP);
	return;
}

/*------------------------------------------------------------------------
 *  wait  -  Cause current process to wait on a semaphore
 *------------------------------------------------------------------------
 */
syscall	wait(
	  sid32		sem		/* Semaphore on which to wait  */
	)
{
	intmask mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	struct	sentry *semptr;		/* Ptr to sempahore table entry	*/

	mask = disable();
	if (isbadsem(sem)) {
		restore(mask);
		return SYSERR;
	}

	semptr = &semtab[sem];
	if (semptr->sstate == S_FREE) {
		restore(mask);
		return SYSERR;
	}

	if (--(semptr->scount) < 0) {		/* If caller must block	*/
		prptr = &proctab[currpid];
		prptr->prstate = PR_WAIT;	/* Set state to waiting	*/
		prptr->prsem = sem;		/* Record semaphore ID	*/
		enqueue(currpid,semptr->squeue);/* Enqueue on semaphore	*/
		resched();
	}

	restore(mask);
	return OK;
}

/*------------------------------------------------------------------------
 *  userret  -  Called when a process returns from the top-level function
 *------------------------------------------------------------------------
 */
void	userret(void)
{
	kill(getpid());			/* Force process to exit */
}

/*------------------------------------------------------------------------
 *  unsleep  -  Internal function to remove a process from the sleep
 *		    queue prematurely.  The caller must adjust the delay
 *		    of successive processes.
 *------------------------------------------------------------------------
 */
status	unsleep(
	  pid32		pid		/* ID of process to remove	*/
        )
{
	intmask	mask;			/* Saved interrupt mask		*/
        struct	procent	*prptr;		/* Ptr to process's table entry	*/

        pid32	pidnext;		/* ID of process on sleep queue	*/
					/*   that follows the process	*/
					/*   which is being removed	*/

	mask = disable();

	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	/* Verify that candidate process is on the sleep queue */

	prptr = &proctab[pid];
	if ((prptr->prstate!=PR_SLEEP) && (prptr->prstate!=PR_RECTIM)) {
		restore(mask);
		return SYSERR;
	}

	/* Increment delay of next process if such a process exists */

	pidnext = queuetab[pid].qnext;
	if (pidnext < NPROC) {
		queuetab[pidnext].qkey += queuetab[pid].qkey;
	}

	getitem(pid);			/* Unlink process from queue */
	restore(mask);
	return OK;
}

/*------------------------------------------------------------------------
 *  suspend  -  Suspend a process, placing it in hibernation
 *------------------------------------------------------------------------
 */
syscall	suspend(
	  pid32		pid		/* ID of process to suspend	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	pri16	prio;			/* Priority to return		*/

	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)) {
		restore(mask);
		return SYSERR;
	}

	/* Only suspend a process that is current or ready */

	prptr = &proctab[pid];
	if ((prptr->prstate != PR_CURR) && (prptr->prstate != PR_READY)) {
		restore(mask);
		return SYSERR;
	}
	if (prptr->prstate == PR_READY) {
		getitem(pid);		    /* Remove a ready process	*/
					    /*   from the ready list	*/
		prptr->prstate = PR_SUSP;
	} else {
		prptr->prstate = PR_SUSP;   /* Mark the current process	*/
		resched();
	}
	prio = prptr->prprio;
	restore(mask);
	return prio;
}


#define	MAXSECONDS	2147483		/* Max seconds per 32-bit msec	*/

/*------------------------------------------------------------------------
 *  sleep  -  Delay the calling process n seconds
 *------------------------------------------------------------------------
 */
syscall	sleep(
	  int32	delay		/* Time to delay in seconds	*/
	)
{
	if ( (delay < 0) || (delay > MAXSECONDS) ) {
		return SYSERR;
	}
	sleepms(1000*delay);
	return OK;
}

/*------------------------------------------------------------------------
 *  sleepms  -  Delay the calling process n milliseconds
 *------------------------------------------------------------------------
 */
syscall	sleepms(
	  int32	delay			/* Time to delay in msec.	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/

	if (delay < 0) {
		return SYSERR;
	}

	if (delay == 0) {
		yield();
		return OK;
	}

	/* Delay calling process */

	mask = disable();
	if (insertd(currpid, sleepq, delay) == SYSERR) {
		restore(mask);
		return SYSERR;
	}

	proctab[currpid].prstate = PR_SLEEP;
	resched();	
	
	restore(mask);
	return OK;
}

/*------------------------------------------------------------------------
 *  resume  -  Unsuspend a process, making it ready
 *------------------------------------------------------------------------
 */
pri16	resume(
	  pid32		pid		/* ID of process to unsuspend	*/
	)
{

	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	pri16	prio;			/* Priority to return		*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return (pri16)SYSERR;
	}
	prptr = &proctab[pid];
	if (prptr->prstate != PR_SUSP) {
		restore(mask);
		return (pri16)SYSERR;
	}
	prio = prptr->prprio;		/* Record priority to return	*/
	restore(mask);
	ready(pid);
	
	return prio;
}

struct	defer	Defer;

/* Reschedule is implemented on Cortex's PendSV call
 * The __attribute__((naked)) makes sure the compiler doesn't
 * places registers on the stack
 *
 * A PendSV is similar to an exception or interrupt
 * A stack frame is pushed on entry and popped
 * on exit BY PROCESSOR. */

/* A pendSV call before entering this handler
 * places the following on the stack 
 * 
 * R0 <- args[0]
 * R1 <- args[1]
 * R2 
 * R3
 * R12
 * LR <- Not a real LR, A certain 'mask'
 * PC 
 * PSR <- Status of processor before call
 *
 * */
void  resched(void){
	char asid; 
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;

	
	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
        return;
	}

	ptold = &proctab[currpid];

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (nonempty(readylist) && (ptold->prprio > firstkey(readylist))) {
			/*asm volatile ("mov r0, %0" : : "r" (restorestk));
			asm volatile("ldmia r0!, {r4-r11} ");
			asm volatile ("msr psp, r0");
		        asm volatile (
                	 "ldr r0, =0xFFFFFFFD" "\n\t"
	                 "mov lr, r0" "\n\t"
	                 "bx lr");*/
			return;
		}

		/* Old process will no longer remain current */
		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	/* Force context switch to highest priority ready process */
	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
	/* Old process returns here when resumed */
	/*asm volatile ("mov r0, %0" : : "r" (ptnew->prstkptr));
	asm volatile("ldmia r0!, {r4-r11} ");
	asm volatile ("msr psp, r0");
        asm volatile (
                 "ldr r0, =0xFFFFFFFD" "\n\t"
                 "mov lr, r0" "\n\t"
                 "bx lr");*/
	asid = currpid & 0xff;
    ctxsw(&ptold->prstkptr, &ptnew->prstkptr, asid);
}
/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}

qid16	readylist;			/* Index of ready list		*/

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
	prptr->prstate = PR_READY;
	insert(pid, readylist, prptr->prprio);
	
	resched();

	return OK;
}

/*------------------------------------------------------------------------
 *  exit  -  Cause the calling process to exit
 *------------------------------------------------------------------------
 */
void	exit(int r)
{
	kill(getpid());		/* Kill the current process */
}
