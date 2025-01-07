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

 
void ilde(){

    while(1){
    	 yield();
    }
} 

void ilde2(){

    while(1){
    	printf("		ilde2\n");
    	//yield();
        sleepms(20);
    }
}


uint32_t fibonacci_series(uint32_t n) {
    if (n <= 0) {
        printf("Fibonacci(0) = 0\n");
        return 0;
    }
    uint32_t a = 0, b = 1, temp;
    printf("Fibonacci(0) = %d\n", a);
    if (n > 1) {
        printf("Fibonacci(1) = %d\n", b);
    }
    for (uint32_t i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
        printf("Fibonacci(%d) = %d\n", i, b);
    }
    return b;
}


static inline uint32_t read_mcycle() {
    uint32_t cycles;
    asm volatile("rdcycle %0" : "=r"(cycles));
    return cycles;
}

// Función de prueba para medir
void test_function() {
    volatile int sum = 0;
    for (int i = 0; i < 1000; i++) {
        sum += i;
    }
}



void task1(){
	uint32_t start, end, elapsed;
    
    // Leer contador antes de ejecutar la función
    start = read_mcycle();

    // Llamar a la función de prueba
    test_function();

    // Leer contador después de ejecutar la función
    end = read_mcycle();

    // Calcular ciclos utilizados
    elapsed = end - start;

    // Mostrar resultado
    printf("Ciclos usados: %d\n", elapsed);
}

void task2(){
	uint32_t n = 30; // Cambia este valor para probar diferentes longitudes de la serie
    uint64_t start, end, elapsed;

    // Leer contador antes de ejecutar la función
    start = read_mcycle();

    // Calcular e imprimir la serie de Fibonacci
    uint32_t result = fibonacci_series(n);

    // Leer contador después de ejecutar la función
    end = read_mcycle();

    // Calcular ciclos utilizados
    elapsed = end - start;

    // Mostrar resultados finales
    printf("\nÚltimo Fibonacci(%d) = %d\n", n, result);
    printf("Ciclos usados: %d\n", elapsed);

}

void cpu_test(uint32_t iterations) {
    uint32_t start, end, elapsed;
    volatile double result = 0.0; // Uso de volatile para evitar optimización excesiva
    uint32_t print_interval = iterations / 10; // Imprimir cada 10% de progreso

    printf("Iniciando test de CPU con %d iteraciones...\n", iterations);

    // Leer el contador inicial
    start = read_mcycle();

    // Realizar cálculos intensivos
    for (uint32_t i = 1; i <= iterations; i++) {
        // Cálculo arbitrario (puedes reemplazar por otro)
        result += (double)i * 1.61803398875; // Multiplicación con la constante Phi

        // Imprimir progreso en intervalos
        if (i % print_interval == 0) {
            printf("Progreso: %d/%d iteraciones completadas\n", i, iterations);
        }
    }

    // Leer el contador final
    end = read_mcycle();

    // Calcular ciclos usados
    elapsed = end - start;

    // Mostrar resultados
    printf("\nTest de CPU completado.\n");
   // printf("Resultado final: %d\n", result);
    printf("Ciclos usados: %d\n", elapsed);
    printf("Ciclos por iteración: %d\n", (double)elapsed / iterations);
}


void test(){

    task1();
    task2();
    cpu_test(1000000);
    uint32_t count=0;
	while(1){
        printf("riscv32 ......... %d\n",count);
        count++;
        sleepms(100);
	}
}


int nullprocess(void) {
kprintf("init process\n");
//resume(create(ilde, 2048 ,3, "ilde", 0));
//resume(create(ilde2, 2048, 1, "ilde2", 0));
resume(create(test, 2048, 1, "ilde2", 0));
//resume(create(shell, 2048, 1, "shell", 0));

return 0;
}

 
void	nulluser(){
uint32	free_mem;	
kprintf ("Build date: %s %s\n\n", __DATE__, __TIME__);
disable();
sysinit();
preempt = QUANTUM;
 

int pid=create(ilde, 512, 1, "ilde", 0);
struct procent * prptr = &proctab[pid];
prptr->prstate = PR_CURR;
//ready(create(ilde, 2048, 3, "ilde", 0));
enable();
ready(create(nullprocess, 2048, 3, "start process", 0));
//nullprocess();
for(;;){
	//kprintf("riscv32 ......... \n");
	//MDELAY(100);
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
