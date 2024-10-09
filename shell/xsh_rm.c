/* xsh_echo.c - xsh_echo */

#include <os.h>
 


shellcmd xsh_rm(int nargs, char *args[]){

return 0;
}


#if 0
#include <os.h>
#include <littlefs.h>


shellcmd xsh_rm(int nargs, char *args[])
{
    int r;
    char *tmp=full_path(args[1]);
    if (tmp==NULL){
        update_path();
        return -1;
    }

    if(!disk.exist(tmp)){
       printf("%s file not found!\n",tmp );
       update_path();
       return -1;
    }
    if((r=disk.remove(tmp))<0){
        printf("error (%d)\n",r);
        update_path();
        return -1;
    }
    printf("%s\n",tmp );
    update_path();

	    #if 0
    FILE* fd;
    char *tmp=full_path((char*)args[1]);
    if (tmp==NULL)return -1;
    if (!(fd = fopen(tmp,"r"))){
        printf("not found %s\n",tmp);
        return -1;
    }
    fclose(fd);
    if(remove(tmp)<0){
       printf("error remove \n");
    }
    #endif
	return 0;
}
#endif