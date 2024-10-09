/* xsh_echo.c - xsh_echo */
#if 1
#include <os.h>
#include <stdio.h>
#include <timer.h>

 

shellcmd xsh_tt(int nargs, char *args[])
{
	#if 0
    int32	i;			/* walks through args array	*/
    char *input=readfile(args[1]);
    if(input==NULL)return 0;
    struct vm *machine = vm_new((struct bytecode *)input);
    //struct vm *machine = vm_new(code);

    hexDump2(0,input,300);
    int err = vm_run(machine);
    if (err) {
        printf("Error executing bytecode: %d\n", err);
        goto exit_monkey;
    }
    
    exit_monkey:
 
 
    free(input);
    update_path();
    #endif



    long n = 100000000; // Número de iteraciones
    long sum = 0;

    // Iniciar la medición del tiempo
    t_time start = timer_get_mtime();//timer_now();

    // Algoritmo a medir (suma de los primeros n números)
    for (long i = 1; i <= n; i++) {
        sum += i;
    }

    // Finalizar la medición del tiempo
    t_time end = timer_get_mtime();//timer_now();

    // Calcular el tiempo transcurrido
    t_time time_spent =(end - start);// / CONFIG_CPU_HZ;

    // Mostrar resultados
    printf("La suma de los primeros %d números es: %d\n", n, sum);
    printf("Tiempo transcurrido: %d segundos\n", time_spent);


   return 0;
    while(1){
        kprintf("test\n");
        sleepms(1000);
    }

 
 
	return 0;
}
#endif