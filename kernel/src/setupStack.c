/**
 * @file setupStack.c
 */
/* Embedded Xinu, Copyright (C) 2018.  All rights reserved. */

#include <os.h>
#include <csr.h>

#define CONTEXT_WORDS 23

/* First 8 arguments pass via a0-a7; the rest spill onto the stack.  */
#define MAX_REG_ARGS 8


char *str[]={"hola","daniel"};


void *setupStack(void *stackaddr, void *procaddr,
                 void *retaddr, uint nargs, va_list ap)
{
    uint spilled_nargs;
    uint32 *saddr = stackaddr;

    /* Construct the context record for the new thread.  */
    saddr -= CONTEXT_WORDS;

    /* Possibly skip a word to ensure the stack is aligned on 16-byte boundary
     * after the new thread pops off the context record.  */
    while ((uint32)saddr & 0xf)
    {
    saddr = (void*)((uint32)saddr) - 1;
    }
    uint i = 0;
    /* Arguments passed in registers (part of context record)  */
    /* program counter */
    saddr[0] = (uint32)procaddr;
    /* x1 - return address */
    saddr[1] = (uint32)retaddr;
    /* Interrupt enabled flag in mstatus set as well previous
     * privilege mode to macine mode and previous interrupt enabled
     * flag to true so mret could be used to properly restore the
     * machine status as well as to jump to newly created thread
     */
    saddr[2] = MSTATUS_MIE | MSTATUS_MPP_M | MSTATUS_MPIE;
    /* s0 - s11 */
    saddr[3] = 0;
    saddr[4] = 0;
    saddr[5] = 0;
    saddr[6] = 0;
    saddr[7] = 0;
    saddr[8] = 0;
    saddr[9] = 0;
    saddr[10] = 0;
    saddr[11] = 0;
    saddr[12] = 0;
    saddr[13] = 0;
    saddr[14] = 0;
    
    saddr[15] = nargs;
    i=16;
    for (int j = 0; j < nargs; ++j)
    {
        saddr[i] = (uint32)va_arg(ap, uint32);
        i++;
    }

   /* for (; i < 16 ; i++)
    {
        saddr[i] = 0;
    }*/
    //saddr[i++]=nargs;
    /* register arguments (as part of context) and rest of arguments
     * that did not fit into the registers  */
    /*for (int j=0; j < MAX_REG_ARGS && j < nargs; j++)
    {
        saddr[i++] = va_arg(ap, uint32);
    }*

    /* rest of a registers (if any) */
    /*for (; i < CONTEXT_WORDS ; i++)
    {
        saddr[i] = 0;
    }*/

    /* Return "top" of stack (lowest address). */
    return saddr;
}


void *setupStackFromShell(void *stackaddr, void *procaddr, void *retaddr, uint nargs, char *args[]) {
    uint spilled_nargs;
    uint32 *saddr = (uint32 *)stackaddr;

    // Determine si algún argumento se derramará en la pila (fuera del registro de contexto)
    if (nargs > MAX_REG_ARGS) {
        spilled_nargs = nargs - MAX_REG_ARGS;
        saddr -= spilled_nargs;
    } else {
        spilled_nargs = 0;
    }

    // Construir el registro de contexto para el nuevo hilo
    saddr -= CONTEXT_WORDS;

    // Posiblemente omitir una palabra para asegurar que la pila esté alineada en un límite de 16 bytes
    while ((uintptr_t)saddr & 0xf) {
        saddr = (void *)((uintptr_t)saddr - 1);
    }

    uint i = 0;
    // Argumentos pasados en registros (parte del registro de contexto)
    // Program counter
    saddr[i++] = (uint32)procaddr;
    // x1 - dirección de retorno
    saddr[i++] = (uint32)retaddr;
    // Interrupt flag en mstatus
    saddr[i++] = MSTATUS_MIE | MSTATUS_MPP_M | MSTATUS_MPIE;
    // s0 - s11
    for (; i < 15; i++) {
        saddr[i] = 0;
    }

    saddr[i++]=nargs;
    //saddr[i++] = (uint32)&args;
    // Argumentos en registros (a0 ... a7)
    for (uint j = 0; j < MAX_REG_ARGS && j < nargs; j++) {
        saddr[i++] = (uint32)&args[j];
    }

    /*// Resto de los argumentos que no caben en los registros
    if (nargs > MAX_REG_ARGS) {
        for (uint j = MAX_REG_ARGS; j < nargs; j++) {
            saddr[i++] = (uint32)args[j];
        }
    }*/

    // Rellenar el resto de los registros (si es necesario)
    for (; i < CONTEXT_WORDS; i++) {
        saddr[i] = 0;
    }

    // Devolver "parte superior" de la pila (la dirección más baja)
    return saddr;
}
