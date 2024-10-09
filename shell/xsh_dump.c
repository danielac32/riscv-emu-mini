/* xsh_echo.c - xsh_echo */


#include <os.h>
 


shellcmd xsh_dump(int nargs, char *args[]){

return 0;
}
#if 0

#include <os.h>
#include <littlefs.h>

shellcmd xsh_dump(int nargs, char *args[])
{
 
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

    uint8 *buff;
    buff=malloc(32);
    //char buff[32];
    int part=0;
    FILE *fs=disk.open(tmp,LFS_O_RDWR);
    int size = disk.size(fs);
    printf("size file %d\n",size);
    while(part<size){
         //syscallp.seek(fs,part,LFS_SEEK_SET);
         disk.read(buff,32,fs);
         hexDump2(part,buff,32);
         memset(buff,0,32);
         part += 32;
    }
    disk.close(fs);
    free(buff);


    update_path();



	    #if 0
	FILE* fd;
 
    char *tmp=full_path(args[1]);
    if (tmp==NULL)return -1;
    if (!(fd = fopen(tmp,"r"))){
        printf("%s not found\n", tmp);
        return -1;
    }
    uint8 buff[32];
    int part=0;
    fseek(fd, 0, SEEK_END);
    unsigned int fileLength = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    printf("size file %d\n",fileLength);

    while(part<fileLength){
         //syscallp.seek(fs,part,LFS_SEEK_SET);
         fread(buff, 32,1,fd);
         hexDump2(part,buff,32);
         memset(buff,0,32);
         part += 32;
    }
    fclose(fd);


    update_path();
    #endif
	return 0;
}
#endif