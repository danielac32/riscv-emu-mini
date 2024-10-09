//#include <stdint.h>
#include <syscall.h>
//#include <elf.h>
#include <os.h>
#include <exception.h>
//#include <process.h>
//#include <os.h>
 


struct irq_context *do_syscall(struct irq_context *ctx);

int syscall_os_init(){
    exception_set_syscall_handler(do_syscall);
    return 0;
}

 


struct irq_context *do_syscall(struct irq_context *ctx)
{
    uint32_t a0 = ctx->reg[REG_ARG0 + 0];
    uint32_t a1 = ctx->reg[REG_ARG0 + 1];
    uint32_t a2 = ctx->reg[REG_ARG0 + 2];
    uint32_t a3 = ctx->reg[REG_ARG0 + 3];
    uint32_t a4 = ctx->reg[REG_ARG0 + 4];
    uint32_t a5 = ctx->reg[REG_ARG0 + 5];
    uint32_t n  = ctx->reg[REG_ARG0 + 7];
    switch(n){
        case OS_FREE_HEAP:
           // ctx->reg[REG_RET]=heap_free();
            ctx->pc += 4;
        break;
        case OS_JSON:
            //ctx->reg[REG_RET]=js0n((char *)a0,a1,(char *)a2,a3,(size_t *)a4);
            //ctx->pc += 4;
        break;
        case OS_LOAD_ELF:
             //ctx->reg[REG_RET]=elf_execve((char *)a0,(exec_img *)a1);
             ctx->pc += 4;
        break;
        case OS_CD:
            {
             //cd((FILE *)a0);
             ctx->pc += 4;
            }
        break;
        case OS_SIZE:
             ///ctx->reg[REG_RET]=disk.size((FILE *)a0);
             ctx->pc += 4;
        break;
        case OS_GET_PATH:
            //ctx->reg[REG_RET]=full_path("");
            ctx->pc += 4;
        break;
        case OS_STRUCT_TASK:
            ctx->reg[REG_RET]=&proctab[a0];
            ctx->pc += 4;
        break;
        case OS_UPDATE_PATH:
            {
               // update_path();
                ctx->pc += 4;
            }
        break;
        case OS_GLOBAL_PATH:
            {
                //ctx->reg[REG_RET]=full_path((char *)a0);
                ctx->pc += 4;
            }
        break;

        case OS_EXIST:
             //ctx->reg[REG_RET]=disk.exist((char *)a0);
             ctx->pc += 4;
        break;
        case OS_YIELD:
             ctx->pc =(uint32)&resched;
        break;   
        case OS_SLEEP:
            //sleep(a0);
            ctx->pc += 4;
        break;
        case OS_FREE:
            //free((void *)a0);
            ctx->pc += 4;
        break;
        case OS_MALLOC:
            //ctx->reg[REG_RET] = malloc(a0);
            ctx->pc += 4;
        break;

        case OS_KILL:
            // kill(a0);
             ctx->pc += 4;
        break;
        case OS_NULLPROCESS:
            {
                /*int len;
                int size;
                char jsonp[100];
                char *jsonbuff=loadConfig("shell/shell.json",&size);
                char *val;

                memset(jsonp,0,100);
                val = js0n("loader",0,jsonbuff,size,&len);
                memcpy(jsonp,val,len);
                exec_img ximg;
                int (*p)(int,char **);
                int ret =elf_execve(jsonp,&ximg);
                if(ret == -1){
                    panic("error load\n");
                }
                p = (int *)ret;
                int child = create(p, 4096, 3, "shell", 0);

                task_t *prptr=& thrtab[child];
                prptr->elf = TRUE;
                prptr->img = (void *)ximg.start;
                ready(child);*/
            }
        case OS_PUTC:
            //printf("%c",a0);
            //sem_wait(&uart_tx);
            //UART=a0;
            //sem_signal(&uart_tx);
            //csr_put(a0);
            //ctx->pc += 4;
            //ctx->pc =(uint32)&start;
        break;
        break;
        case OS_PUTS:
            //sem_wait(&uart_tx);
            //print((char *)a0);
            //sem_signal(&uart_tx);
            //printf("%s", (char *)a0);
            //ctx->reg[REG_RET] = 0;
            //ctx->pc += 4;
        break;
        
        case OS_GETC:

        break;
        case OS_GETS:

        break;
        case OS_CLOSE:

        break;
        case OS_OPEN:

        break;
        case OS_READ:

        break;
        case OS_WRITE:

        break;
        case OS_CREATE:
             //ctx->reg[REG_RET]=create((void *)a0, a1, 3, (char *)a2, 0);
             ctx->pc += 4;
        break;
        case OS_READY:
            //ctx->reg[REG_RET]=ready(a0);
            ctx->pc += 4;
        break;
        case OS_SEMA_INIT:

        break;
        case OS_SEMA_SIGNAL:

        break;
        case OS_SEMA_WAIT:

        break;

        case OS_FOPEN:
            {
                //ctx->reg[REG_RET] = disk.open((char *)a0,a1);
                ctx->pc += 4;
            }
        break;
        case OS_FSEEK:
            {
                //ctx->reg[REG_RET] = disk.seek((FILE *)a0,a1,a2);
                ctx->pc += 4;
            }
        break;
        case OS_AVAILABLE:
            {
               // ctx->reg[REG_RET] = disk.available((FILE *)a0);
                ctx->pc += 4;
            }
        break;
        case OS_FREAD:
            {
                //ctx->reg[REG_RET] = disk.read((void *)a0,a1,(FILE *)a2);
                ctx->pc += 4;
            }
        break;
        case OS_FWRITE:
            {
                //ctx->reg[REG_RET] = disk.write((void *)a0,a1,(FILE *)a2);
                ctx->pc += 4;
            }
        break;
        case OS_FCLOSE:
            {
                //ctx->reg[REG_RET] = disk.close((FILE *)a0);
                ctx->pc += 4;
            }
        break;
        case OS_FTELL:
                {
                    //ctx->reg[REG_RET] = disk.tell((FILE *)a0);
                    ctx->pc += 4;
                }
        break;
        case OS_FEOF:

        break;
        case OS_REMOVE:
            {
                //ctx->reg[REG_RET] = disk.remove((char *)a0);
                ctx->pc += 4;
            }
        break;
        case OS_OPENDIR:
            {
                //ctx->reg[REG_RET] = disk.openDir((char *)a0);
                ctx->pc += 4;
            }
        break;
        case OS_READDIR:
            {
                //ctx->reg[REG_RET] = disk.readDir((DIR *)a0,(struct lfs_info *)a1);
                ctx->pc += 4;
            }
        break;
        case OS_CLOSEDIR:
            {
                //ctx->reg[REG_RET] = disk.closeDir((DIR *)a0);
                ctx->pc += 4;
            }
        break;
        case OS_MKDIR:
            {
                //ctx->reg[REG_RET] = disk.mkdir((char *)a0);
                ctx->pc += 4;
            }
        break;
        case OS_RENAME:
            {
                //ctx->reg[REG_RET] = disk.rename((char *)a0,(char *)a1);
                ctx->pc += 4;
            }
        break;
        case OS_FORMAT:
            {
                //ctx->reg[REG_RET] = disk.format((lfs_t *)a0,(struct lfs_config *)a1);
                ctx->pc += 4;
            }
        break;
        case OS_MOUNT:
            {
                //ctx->reg[REG_RET] = disk.mount((lfs_t *)a0,(struct lfs_config *)a1);
                ctx->pc += 4;
            }
        break;
        case OS_UNMOUNT:
            {
                //ctx->reg[REG_RET] = disk.unmount((lfs_t *)a0);
                ctx->pc += 4;
            }
        break;
        case OS_DISK_FREE:
            {
                //ctx->reg[REG_RET] = disk.free();
                ctx->pc += 4;
            }
        break;
        case OS_IS_DIR:

        break;
        case OS_FFLUSH:

        break;
        case OS_FGETC:

        break;
        case OS_FGETCS:

        break;
        case OS_FPUTC:

        break;
        case OS_FPUTCS:

        break;
        case OS_FGETPOS:

        break;
        case OS_NUM_TASK:

        break;
        default:
            kprintf("syscall no implement %x, %d\n",n,n);
            ctx->pc += 4;
        break;
    }
     
    return ctx;
}