/* xsh_echo.c - xsh_echo */
#include <os.h>
 


shellcmd xsh_ls(int nargs, char *args[]){

return 0;
}



#if 0
#include <os.h>
#include <littlefs.h>
 
shellcmd xsh_ls(int nargs, char *args[])
{
	struct lfs_info info;
   // char *tmp=malloc(200);
    char *tmp=full_path("");
    DIR *dir = disk.openDir(tmp);
    if(dir==0){
        printf("not a directory %s",tmp);
    }
    while (disk.readDir(dir, &info)> 0){
        if (strcmp(info.name, ".") && strcmp(info.name, "..") && strcmp(info.name, "//")){
            
            char *tmp=full_path(info.name);
            if(disk.exist(tmp)){
                if (info.type == LFS_TYPE_DIR){
                    printf(" /%s\n",info.name);
                }else{
                    printf(" %s %d \n",info.name,info.size);
                }
            }
            strcpy(path, curdir);
        }
    }

    

    disk.closeDir(dir);
	return 0;
}
#endif