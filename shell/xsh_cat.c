/* xsh_echo.c - xsh_echo */


#include <os.h>
 


shellcmd xsh_cat(int nargs, char *args[]){

return 0;
}


#if 0
#include <os.h>
#include <littlefs.h>


void cat(char *input,char *output) {
    FILE *file;
    char ch;
    //char buffer[256];
    int bytes;
    
    char *tmp=full_path(input);
    if (tmp==NULL){
        update_path();
        return;
    }
    if(!disk.exist(tmp)){
       //printf("%s not found!\n",tmp );
       //update_path();
       file=disk.open(tmp,LFS_O_CREAT);
       disk.close(file);
       //return;
    }


    if(output==NULL){
        file=disk.open(tmp,LFS_O_RDWR);
        while(disk.available(file)){
           disk.read(&ch,1,file);
           //putc(CONSOLE,ch);
           printf("%c",ch );
        }
        disk.close(file);
        printf("\n");
    }else{
        file=disk.open(tmp,LFS_O_CREAT);
        disk.write(output,strlen(output),file);
        //disk.write(" ",1,fs);
        disk.close(file);
    } 
    update_path();
}


shellcmd xsh_cat(int nargs, char *args[])
{

 
    if (nargs == 2) {
        // cat archivo.txt
        cat(args[1],NULL);
        //printf("cat archivo.txt\n");
    } else if (nargs == 4 && strcmp(args[2], ">") == 0) {
        // cat archivo.txt > nuevo_contenido
        cat(args[1], args[3]);
        //printf("%s\n",(char *)&args[3] );

        //printf("cat archivo.txt > nuevo_contenido\n");
    }else {
        printf("Usage: cat archivo.txt [> nuevo_contenido]\n");
    }


   #endif


	