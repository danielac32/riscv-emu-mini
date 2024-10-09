/* xsh_echo.c - xsh_echo */

#include <os.h>
 
#include <elf.h>

extern uint32 load_elf(const char *file);
shellcmd xsh_run(int nargs, char *args[])
{

     #if 0
	char *tmp=full_path((char*)args[1]);
    if (tmp==NULL)return -1;
    FILE *fptr;
    exec_img ximg;
    int32	tmparg;
    int child;
    

   //load_elf(tmp);
    int ret = elf_execve(tmp,&ximg);
    if(ret > 0){
        int (*p) = (int *)ret;
        child = create(p,SHELL_CMDSTK, SHELL_CMDPRIO,tmp, 2, nargs, &args[0]);
        
        struct	procent *prptr;
        prptr = &proctab[child];
        prptr->elf = TRUE;
        prptr->img = ximg.base;
        prptr->size = ximg.size;
        //free(prptr->img);
        //resume(child);

    }else {
        printf("error loading elf process  %d %s\n",ret,tmp);
        return -1;
    }
    
    update_path();
    printf("pid: %d\n", child);
    ready(child);

  #endif
	 
	return 0;
}
