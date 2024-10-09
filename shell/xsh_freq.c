 
#include <os.h>
//#include <stdio.h>

#include <timer.h>

 

volatile uint32_t cycles_start, cycles_end;

// Función para obtener el contador de ciclos
uint32_t get_cycles() {
    uint32_t cycles;
    asm volatile ("rdcycle %0" : "=r"(cycles));
    return cycles;
}

// Función que simula una carga de trabajo
void workload() {
    volatile int i;
    for (i = 0; i < 1000000; i++); // Cambia este número para ajustar la carga
}


shellcmd xsh_freq(int nargs, char *args[])
{
    cycles_start = get_cycles();
    workload();
    cycles_start = get_cycles();
    
    uint32_t cycles_used = cycles_end - cycles_start;

    // Calcular la frecuencia (esto dependerá del tiempo transcurrido)
    // Suponiendo que el tiempo transcurrido es 1 segundo
    uint32_t frequency = cycles_used; // en ciclos por segundo
    printf("%d mhz\n", frequency/1000000);

	return 0;
}
 