/* xsh_echo.c - xsh_echo */

#include <os.h>
#include <stdio.h>


#define VARADDR 0x0800C000 // address that will hold the variable
#define VAR     0x12345670 // variable value


/*------------------------------------------------------------------------
 * xhs_echo - write argument strings to stdout
 *------------------------------------------------------------------------
 */
 
shellcmd xsh_reboot(int nargs, char *args[])
{
    //char *buff=malloc(200);
    
    //sprintf(buff,"hola como estas %d \n",888);
    
    //__syscall(XINU_TEST2,"hola como estas %d \n",888);
    //__syscall(XINU_TEST2,buff);
   	
   	//printf2("hola como estas %d \n",888);
    //printf("%s\n",buff );
    //free(buff);


    /*unlock_flash();
    erase_flash_sector(10);
    write_flash(0x08010000, 0xbebecafe);

    ///write_flash(VARADDR, 0xbebecafe);
        // do read write
    lock_flash();

    unlock_flash();

    uint32 r = read_flash(0x08010000);
    printf2("%08x\n", r);*/

   // uint32 r = syscall3(XINU_TEST2,0,0,0);
    //printf("%d\n", r);

    uint32_t value = 0x7777;  //0x5555 El valor que quieres almacenar
    asm volatile (
        "lui t0, 0x11100       \n"  // Carga los 20 bits superiores (0x11100000) en t0
        //"addi t0, t0, 1      \n"  // Añade los 12 bits inferiores (0x00000001) a t0
        "sw %0, 0(t0)"         // Almacena el valor en la dirección contenida en t0
        :                      // No salida
        : "r" (value)          // Entrada: el valor a almacenar
        : "t0", "memory"       // Clobbers: `t0` y `memory` (ya que estamos modificando memoria)
    );

	return 0;
}
