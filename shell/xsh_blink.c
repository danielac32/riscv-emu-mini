

#include <os.h>
 
shellcmd xsh_blink(int nargs, char *args[]){

return 0;
}



/* xsh_echo.c - xsh_echo */
#if 0
#include <os.h>
#include <stdio.h>
#include "compiler.h"
#include "object.h"
#include "vm.h"
#include <littlefs.h>

/*char *input;
struct lexer lexer;
struct parser parser;
struct program *program;*/
/*------------------------------------------------------------------------
 * xhs_echo - write argument strings to stdout
 *------------------------------------------------------------------------
 */
extern shellcmd xsh_memstat(int nargs, char *args[]);

char *readFile(char *path){
	char *tmp=full_path(path);
    if (tmp==NULL){
        update_path();
        return NULL;
    }
    if(!disk.exist(tmp)){
       printf("%s file not found!\n",tmp );
       update_path();
       return NULL;
    }

    uint8 *buff;
    FILE *fs=disk.open(tmp,LFS_O_RDWR);
    int size = disk.size(fs);
    printf("size file %d\n",size);
    buff = malloc(size+100);
    memset(buff,0,size);
    disk.read(buff,size,fs);
    disk.close(fs);
    buff[size] = '\0';
    return buff;
}

//char *input;
 

shellcmd xsh_blink(int nargs, char *args[])
{
	int32	i;			/* walks through args array	*/
    
   FILE *file;
    
    char *input=readFile("/usr/test.monkey");
    struct lexer lexer = new_lexer(input);
	 struct parser parser = new_parser(&lexer);
	 struct program *program = parse_program(&parser);


	if (parser.errors > 0) {
		for (int8_t i = 0; i < parser.errors; i++) {
			printf("%s\n",parser.error_messages[i]);
		}

		goto exit_monkey;
	}


	struct compiler *compiler = compiler_new();
	int err = compile_program(compiler, program);
	if (err) {
		printf("%s\n",compiler_error_str(err));
		goto exit_monkey;
	}

	struct bytecode *code = get_bytecode(compiler);
    
   struct vm *machine = vm_new(code);
	err = vm_run(machine);
	if (err) {
		printf("Error executing bytecode: %d\n", err);
		goto exit_monkey;
	}
   

    xsh_memstat(0,NULL);
    exit_monkey:
	free_program(program);
	compiler_free(compiler);
	free(code);
	vm_free(machine);
    free(input);
    update_path();
    return 0;
    
}
#endif

/* xsh_echo.c - xsh_echo */
#if 0
#include <os.h>
#include <stdio.h>
#include "compiler.h"
#include "object.h"
#include "vm.h"
#include <littlefs.h>

/*char *input;
struct lexer lexer;
struct parser parser;
struct program *program;*/
/*------------------------------------------------------------------------
 * xhs_echo - write argument strings to stdout
 *------------------------------------------------------------------------
 */

char *readFile(char *path){
   char *tmp=full_path(path);
    if (tmp==NULL){
        update_path();
        return NULL;
    }
    if(!disk.exist(tmp)){
       printf("%s file not found!\n",tmp );
       update_path();
       return NULL;
    }

    uint8 *buff;
    FILE *fs=disk.open(tmp,LFS_O_RDWR);
    int size = disk.size(fs);
    printf("size file %d\n",size);
    buff = malloc(size+100);
    memset(buff,0,size);
    disk.read(buff,size,fs);
    disk.close(fs);
    buff[size] = '\0';
    return buff;
}

//char *input;

int example1(int nargs, char *args[])
{
   printf("example1\n");
   for (int i = 0; i < 10; ++i)
   {
      printf("example1\n");
   }
   return OK;
}
int example0(int nargs, char *args[])
{
   //input=readFile("/usr/test.monkey");
   /*input=readFile("/usr/test.monkey");
    struct lexer lexer = new_lexer(input);
   struct parser parser = new_parser(&lexer);
   struct program *program = parse_program(&parser);*/

    
   printf("example\n");
   resume(create(example1, 512, 1, "ex_0",0));
   receive();
   return OK;
}


shellcmd xsh_blink(int nargs, char *args[])
{
   int32 i;       /* walks through args array   */
    
    FILE *file;
    resume(create(example0, 512, 1, "ex_0",0));
    receive();

   char *input=readFile("/usr/test.monkey");
    struct lexer lexer = new_lexer(input);
    struct parser parser = new_parser(&lexer);
    struct program *program = parse_program(&parser);


   if (parser.errors > 0) {
      for (int8_t i = 0; i < parser.errors; i++) {
         printf("%s\n",parser.error_messages[i]);
      }

      goto exit_monkey;
   }


   struct compiler *compiler = compiler_new();
   int err = compile_program(compiler, program);
   if (err) {
      printf("%s\n",compiler_error_str(err));
      goto exit_monkey;
   }

   struct bytecode *code = get_bytecode(compiler);
    //printf("%d\n", code->instructions->size);
    //hexDump2(0,code,code->instructions->size);
    char *p=malloc(code->instructions->size);
    memcpy(p,code,code->instructions->size);
    //hexDump2(0,code,code->instructions->size);


    char *tmp=full_path("monkey");
    if(disk.exist(tmp)){
       printf("%s file found!\n",tmp );
       update_path();
       goto exit_monkey;
       //return -1;
    }
    
    file=disk.open(tmp,LFS_O_CREAT);
    disk.write(p,code->instructions->size,file);
    disk.close(file);
    free(p);
    



    char *bc=readFile("monkey");
    hexDump2(0,bc,code->instructions->size);
    struct vm *machine = vm_new((struct bytecode *)bc);
    //struct vm *machine = vm_new((struct bytecode *)p);
   //struct vm *machine = vm_new(code);
   err = vm_run(machine);
   if (err) {
      printf("Error executing bytecode: %d\n", err);
      goto exit_monkey;
   }
    free(bc);
    exit_monkey:
   free_program(program);
   compiler_free(compiler);
   free(code);
   vm_free(machine);
    free(input);
    update_path();
    return 0;
    
}
#endif