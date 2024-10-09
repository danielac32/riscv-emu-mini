/* xsh_echo.c - xsh_echo */

#include <os.h>
 


shellcmd xsh_mkdir(int nargs, char *args[]){

return 0;
}



#if 0
#include <os.h>
#include <littlefs.h>

shellcmd xsh_mkdir(int nargs, char *args[])
{
   
    int r;
    char *tmp=full_path(args[1]);
    if (tmp==NULL){
        update_path();
        return -1;
    }
    if(disk.exist(tmp)){
       printf("%s Dir found!\n",tmp );
       update_path();
       return -1;
    }
    
    if((r=disk.mkdir(tmp))<0){
       printf("error (%d)\n",r);
       update_path();
       return -1;
    }
    update_path();

    
   #if 0
	FL_DIR dirstat;
    char *tmp=full_path((char*)args[1]);
    if (tmp==NULL)return -1;
    if(fl_opendir(tmp,&dirstat)){
        fl_closedir(&dirstat);
        printf("dir found %s\n",tmp);
        return 0;
    }
    
    mkdir(tmp);
    #endif
	return 0;
}
#endif