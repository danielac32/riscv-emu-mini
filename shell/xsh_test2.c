
#include <os.h>
 
uint32 fibonacci_recursivo(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci_recursivo(n - 1) + fibonacci_recursivo(n - 2);
}
uint32 factorial_recursivo(int n) {
    if (n < 0) return 0; // Factorial no definido para números negativos
    if (n == 0 || n == 1) return 1;
    return n * factorial_recursivo(n - 1);
}


int ex1(int nargs, char *args[])
{
   //printf("example1\n");

  printf("%s: %d\n",args[0],atoi(args[1]));
  /* for (int i = 0; i < 10; ++i)
   {
      printf("%d\n",i);
   }*/
   printf("factorial: %u\n",factorial_recursivo(atoi(args[1])) & 0xffffffff);
   printf("___________________________________________\n");
   return OK;
}

int ex0(int nargs, char *args[])
{
   
   //printf("%d\n",nargs );
  // for (int i = 0; i < nargs; ++i)
   //{
     //printf("->%s\n",args[i] );
   //}


   //printf("creado tarea: %d\n",atoi(args[0]));
   resume(create(ex1, 4096*2, 1, "ex_1",2,"creado tarea",args[0]));
  // receive();
   return OK;
}


/*int test(int nargs, char *args[]){
printf("aqui: %d\n",nargs);
for (int i = 0; i < nargs; ++i)
{
  printf("->%s\n", args[i]);
}
return 0;
}
*/

shellcmd xsh_test2(int nargs, char *args[]){

  //resume(create(test, 2048, 10, "test",3,"daniel","quintero","astrid quintero"));
 // receive();
  char str[10];
  for (int i = 0; i < 1000; ++i)
  {

    itoa(i, str, 10,10);
    resume(create(ex0, 4096, i, "ex_0",2,str,"daniel"));
    receive();
  }
  
  return 0;
}