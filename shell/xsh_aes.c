
#include <os.h>
#include <aes.h>


shellcmd xsh_aes(int nargs, char *args[]){

return 0;
}
#if 0
#include <os.h>
#include <littlefs.h>

#include <aes.h>

#define BUF_LEN 64
void generate_random_name(char *name, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    int i;

    for (i = 0; i < length; i++) {
        int key = rand() % (sizeof(charset) - 1); // -1 to exclude the null terminator
        name[i] = charset[key];
    }
    name[length] = '\0'; // Null-terminate the string
}


void encrypt(char *s){
	//FILE *outfile;
    FILE *fs;
    uint8 *buff;
    //uint8 *b16;
    ///int b16_len;
    int buff_len;
    int part=0;
    char *tmp=full_path(s);
 
    if(!disk.exist(tmp)){
       printf("%s file not found!\n",tmp );
       update_path();
       return;
    }

    buff=malloc(BUF_LEN + 1);
    
    fs=disk.open(tmp,LFS_O_RDONLY);
    int size = disk.size(fs);
    printf("size file %d\n",size);
    char *p = malloc(size + 1);
    
    //disk.seek(outfile,part,LFS_SEEK_SET);
    while(part<size){
         //syscallp.seek(fs,part,LFS_SEEK_SET);
         buff_len=disk.read(buff,BUF_LEN,fs);
         AES256_encrypt(buff); 
         //AES256_decrypt(buff);
         //printf("%s\n", buff);
         memcpy(&p[part],buff,buff_len);
         memset(buff,0,BUF_LEN);
         part += buff_len;
    }
    //hexDump2(0,p,size);
    disk.close(fs);
    
    char name[16];
    generate_random_name(name, 4);
    strcat(name,".aes");
    fs=disk.open(full_path(name),LFS_O_CREAT);
    disk.write(p,size,fs);
    disk.close(fs);

    free(p);
    free(buff);
    update_path();
}

void decrypt(char *s){
//FILE *outfile;
    FILE *fs;
    uint8 *buff;
    //uint8 *b16;
    ///int b16_len;
    int buff_len;
    int part=0;
    char *tmp=full_path(s);
 
    if(!disk.exist(tmp)){
       printf("%s file not found!\n",tmp );
       update_path();
       return;
    }
 
    buff=malloc(BUF_LEN + 1);
    
    fs=disk.open(tmp,LFS_O_RDONLY);
    int size = disk.size(fs);
    printf("size file %d\n",size);
    char *p = malloc(size + 1);
    
 
    while(part<size){
 
         buff_len=disk.read(buff,BUF_LEN,fs);
         AES256_decrypt(buff); 
  
         memcpy(&p[part],buff,buff_len);
         memset(buff,0,BUF_LEN);
         part += buff_len;
    }
    //hexDump2(0,p,size);
    printf("%s\n", p);
    disk.close(fs);
    
    /*char name[16];
    generate_random_name(name, 4);
    strcat(name,".aes");
    fs=disk.open(name,LFS_O_CREAT);
    disk.write(p,size,fs);
    disk.close(fs);*/

    free(p);
    free(buff);
    update_path();
}

shellcmd xsh_aes(int nargs, char *args[]){

    


    if (nargs < 3) {
    	printf("Usage: aes -e archivo.txt [ nuevo.txt ]\n");
    	//return -1;
    }

    Byte key[32]={1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0,
          0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20,
          0x21, 0x22, 0x23, 0x24, 0x25, 0x26};  
    AES_setKey(key);


    if(!strcmp(args[1],"-e")){
        printf("encriptar %s\n", args[2]);
        encrypt(args[2]);
    }
    if(!strcmp(args[1],"-d")){
    	printf("desencriptar %s\n", args[2]);
    	decrypt(args[2]);
    }
    

    return 0;


#if 0
    char *tmp=full_path("/shell/shell.json");
 
    if(!disk.exist(tmp)){
       printf("%s file not found!\n",tmp );
       update_path();
       return -1;
    }
    
    /*char *tmp2=full_path(args[2]);
    if(disk.exist(tmp2)){
       printf("%s file exist!\n",tmp2 );
       update_path();
       return -1;
    }*/

    
    Byte key[32]={1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0,
          0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20,
          0x21, 0x22, 0x23, 0x24, 0x25, 0x26};  
    AES_setKey(key);


    buff=malloc(BUF_LEN + 1);
    //b16=malloc(BUF_LEN + 1);
 
    
    fs=disk.open(tmp,LFS_O_RDONLY);
    //outfile=disk.open(full_path("b16f.b16"),LFS_O_CREAT | LFS_O_RDWR);
 
    int size = disk.size(fs);
    printf("size file %d\n",size);
    char *p = malloc(size + 1);
    
    //disk.seek(outfile,part,LFS_SEEK_SET);
    while(part<size){
         //syscallp.seek(fs,part,LFS_SEEK_SET);
         buff_len=disk.read(buff,BUF_LEN,fs);
         AES256_encrypt(buff); 
         //AES256_decrypt(buff);
         //printf("%s\n", buff);
         memcpy(&p[part],buff,buff_len);
         memset(buff,0,BUF_LEN);
         part += buff_len;
    }
    //hexDump2(0,p,size);
    

    //disk.close(outfile);
    disk.close(fs);
    
    char name[16];
    generate_random_name(name, 4);
    strcat(name,".aes");
    fs=disk.open(name,LFS_O_CREAT);
    disk.write(p,size,fs);
    disk.close(fs);

    free(p);
    free(buff);
    //free(b16);

    update_path();
#endif
return 0;
}
#endif