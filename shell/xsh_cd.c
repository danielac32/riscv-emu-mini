/* xsh_echo.c - xsh_echo */


#include <os.h>
 


shellcmd xsh_cd(int nargs, char *args[]){

return 0;
}

#if 0
#include <os.h>
#include <littlefs.h>


shellcmd xsh_cd(int nargs, char *args[])
{

  char *s=args[1];
  if (strcmp(s, ".") == 0){
     goto cd_done;
   }else if (strcmp(s, "..") == 0) {
      if (strcmp(curdir, "/") == 0) {
          printf("not a directory\n");
          //goto cd_done;//
          return -1;
      }
      strcpy(path, curdir);
      char* cp = strrchr(path, '/');
        if (cp == NULL)
            cp = curdir;
        *cp = 0;
        cp = strrchr(path, '/');
        if (cp != NULL)
            *(cp + 1) = 0;
        goto cd_done;
   }
   full_path(s);
   if(!disk.exist(s)){
       printf("%s not found!\n",s );
       strcpy(path, curdir);
       return -1;
   }
   printf("%s\n",path );
    
   cd_done:
    strcpy(curdir, path);
    if (curdir[strlen(curdir) - 1] != '/'){
        strcat(curdir, "/");
    }
	return 0;
}
#endif