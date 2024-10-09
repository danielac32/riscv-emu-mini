/* xsh_echo.c - xsh_echo */


#include <os.h>
 


shellcmd xsh_touch(int nargs, char *args[]){

return 0;
}


#if 0
#include <os.h>
#include <littlefs.h>


shellcmd xsh_touch(int nargs, char *args[])
{
    FILE *file;

    char *tmp=full_path(args[1]);
    if (tmp==NULL){
        update_path();
        return -1;
    }

    if(disk.exist(tmp)){
       printf("%s file found!\n",tmp );
       update_path();
       return -1;
    }
    
    file=disk.open(tmp,LFS_O_CREAT);
    disk.close(file);
    update_path();


    #if 0
	FILE* fd;
    char *tmp=full_path((char*)args[1]);
    if (tmp==NULL)return -1;
    if ((fd = fopen(tmp,"r"))){
        printf("file found %s\n",tmp);
        fclose(fd);
        return -1;
    }
    fd = fopen(tmp,"w");
    fclose(fd);
    #endif
	return 0;
}
#endif