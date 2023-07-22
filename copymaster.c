#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <time.h>
#include <dirent.h>

#include "options.h" 


void FatalError(char c, const char* msg, int exit_status);
void PrintCopymasterOptions(struct CopymasterOptions* cpm_options);


int main(int argc, char* argv[])
{
    
    struct CopymasterOptions cpm_options = ParseCopymasterOptions(argc, argv);

    //-------------------------------------------------------------------
    // Kontrola hodnot prepinacov
   if(cpm_options.fast==1&&cpm_options.slow==1){
        fprintf(stderr,"KONFLIKT PREPINACOV\n");
        exit(1);
    }else if(cpm_options.create==1&&cpm_options.overwrite==1){
        fprintf(stderr,"KONFLIKT PREPINACOV\n");
        exit(1);
    }else if(cpm_options.append && cpm_options.create){
        fprintf(stderr, "CHYBA PREPINACOV\n");
        exit(EXIT_FAILURE);
    }else if(cpm_options.append && cpm_options.link){
            fprintf(stderr, "CHYBA PREPINACOV\n");
            exit(EXIT_FAILURE);
    }else if(cpm_options.link && cpm_options.overwrite){
            fprintf(stderr, "CHYBA PREPINACOV\n");
            exit(EXIT_FAILURE);
    }else if(cpm_options.link && cpm_options.lseek){
            fprintf(stderr, "CHYBA PREPINACOV\n");
            exit(EXIT_FAILURE);
    }else if(cpm_options.create && cpm_options.lseek){
            fprintf(stderr, "CHYBA PREPINACOV\n");
            exit(EXIT_FAILURE);
    }else if(cpm_options.sparse && argc != 4){
        fprintf(stderr, "CHYBA PREPINACOV\n");
        exit(EXIT_FAILURE);
    }


    // Osetrenie prepinacov pred kopirovanim
   mode_t oldmask=umask(S_IRWXG);
  // Osetrenie prepinacov pred kopirovanim
   if (cpm_options.umask == 1)
    {
      int um, um1, um2, gm, gm1, gm2, om, om1, om2;
      mode_t rights = 0;
      int f = 0;
      int u, g, o;
      int plus, minus;
     int octalNumber = 0, i = 1;

    while (oldmask != 0)
    {
        octalNumber += (oldmask % 8) * i;
        oldmask /= 8;
        i *= 10;
    }
oldmask=octalNumber;
     mode_t desa,dec,jedni;
      if(oldmask>=100)
      { 
          dec = oldmask/100;
          desa = (oldmask%100)/10;
          jedni = oldmask%10;
      }
      if(oldmask<=99)
      { desa = oldmask/10;
        jedni = oldmask%10;
      }
      if(oldmask<=9){jedni = oldmask%10;}
      if(dec==1){um2=1;}
      if(dec==2){um=1;}
      if(dec==3){um=1;um2=1;}
      if(dec==4){um1=1;}
      if(dec==5){um1=1;um2=1;}
      if(dec==6){um1=1;um=1;}
      if(dec==6){um=1;um1=1;um2=1;}
      if(desa==1){gm2=1;}
      if(desa==2){gm=1;}
      if(desa==3){gm=1;gm2=1;}
      if(desa==4){gm1=1;}
      if(desa==5){gm1=1;gm2=1;}
      if(desa==6){gm1=1;gm=1;}
      if(desa==6){gm=1;gm1=1;gm2=1;}
      if(jedni==1){om2=1;}
      if(jedni==2){om=1;}
      if(jedni==3){om=1;om2=1;}
      if(jedni==4){om1=1;}
      if(jedni==5){om1=1;om2=1;}
      if(jedni==6){om1=1;om=1;}
      if(jedni==6){om=1;om1=1;om2=1;}
      //printf ("%d\n",dec);
      //printf ("%d\n",desa);
      //printf ("%d\n",jedni);
     // printf ("%d\n",oldmask);
      
      for (size_t j = 0; j < strlen (cpm_options.umask_options[f]); j++){
	  for (int i = 0; i < 3; i++){
	      if (i == 0 && cpm_options.umask_options[f][i] == 'u'){
		  u = 1;
		}
	      else if (i == 0 && cpm_options.umask_options[f][i] == 'g'){
		  g = 1;
		}
	      else if (i == 0 && cpm_options.umask_options[f][i] == 'o')
		{
		  o = 1;
		}
		
		
	      if (i == 1 && cpm_options.umask_options[f][i] == '+'){
		  plus = 1;
		}
	      else if (i == 1 && cpm_options.umask_options[f][i] == '-'){
		  minus = 1;
		}
	      
	      if (i == 2 && cpm_options.umask_options[f][i] == 'w' && u == 1&& minus == 1&&um!=1){		///minus
		  rights = rights + 200;
		  um = 1;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'r'&& u == 1 && minus == 1&&um1!=1){
		  rights = rights + 400;
		  um1 = 1;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'x'&& u == 1 && minus == 1&&um2!=1){
		  rights = rights + 100;
		  um2 = 1;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'w'&& g == 1 && minus == 1&&gm!=1){
		  rights = rights + 20;
		  gm = 1;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'r'&& g == 1 && minus == 1&&gm1!=1){
		  rights = rights + 40;
		  gm1 = 1;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'x'&& g == 1 && minus == 1&&gm2!=1){
		  rights = rights + 10;
		  gm2 = 1;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'w'&& o == 1 && minus == 1&&om!=1){
		  rights = rights + 2;
		  om = 1;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'r'&& o == 1 && minus == 1&&om1!=1)
		{
		  rights = rights + 4;
		  om1 = 1;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'x'&& o == 1 && minus == 1&&om2!=1){
		  rights = rights + 1;
		  om2 = 1;
		}
	      if (i == 2 && cpm_options.umask_options[f][i] == 'w' && u == 1 && plus == 1 && um == 1){		///plus
		  rights = rights - 200;
		  um = 0;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'r'&& u == 1 && plus == 1 && um1 == 1){
		  rights = rights - 400;
		  um1 = 0;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'x'&& u == 1 && plus == 1 && um2 == 1){
		  rights = rights - 100;
		  um2 = 0;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'w'&& g == 1 && plus == 1 && gm == 1){
		  rights = rights - 20;
		  gm = 0;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'r'&& g == 1 && plus == 1 && gm1 == 1){
		  rights = rights - 40;
		  gm1 = 0;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'x'&& g == 1 && plus == 1 && gm2 == 1){
		  rights = rights - 10;
		  gm2 = 0;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'w'&& o == 1 && plus == 1 && om == 1){
		  rights = rights - 2;
		  om = 0;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'r'&& o == 1 && plus == 1 && om1 == 1){
		  rights = rights - 4;
		  om1 = 0;
		}
	      else if (i == 2 && cpm_options.umask_options[f][i] == 'x'&& o == 1 && plus == 1 && om2 == 1){
		  rights = rights - 1;
		  om2 = 0;
		}
	    }
	  u = 0, g = 0, o = 0, minus = 0, plus = 0;
	  f++;
	}
oldmask=oldmask-rights;

    //  umask (rights);
    //  printf ("%d\n", rights);
    //  printf ("%d", oldmask);
     // int des1 =
	//open (argv[argc - 1], O_CREAT | O_WRONLY,
	   //   S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
   // return 0;
    }
    //-------------------------------------------------------------------

    // Vypis hodnot prepinacov odstrante z finalnej verzie
    
    PrintCopymasterOptions(&cpm_options);
    
    //-------------------------------------------------------------------
    // Osetrenie prepinacov pred kopirovanim
  int  fd=open(argv[argc-2],O_EXCL); //-1 if no file and 3 if have file
  
  
  int  fd1=open(argv[argc-1],O_EXCL); //-1 if no file and 4 if have file
 
    //-------------------------------------------------------------------
    
    if (cpm_options.fast && cpm_options.slow) {
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        exit(EXIT_FAILURE);
    }
    
    // TODO Nezabudnut dalsie kontroly kombinacii prepinacov ...
    
    //-------------------------------------------------------------------
    // Kopirovanie suborov
    int des,des1,n;
    char buf[1024000];
    int c=-1;
    int m=1;
    //struct stat buff; 
  ////////////////////////////////////////////////////////////////
    //default option
   
    ////////////////////////////////////////////////////////////////
    if(cpm_options.slow==1){//slow
        des=open(argv[argc-2],O_RDONLY);
        des1 = open(argv[argc-1],O_CREAT|O_WRONLY|O_TRUNC);
        c=0;
        m=1;
    }
    ////////////////////////////////////////////////////////////////
    if(cpm_options.fast==1){//fast
        des=open(argv[argc-2],O_RDONLY);
        des1 = open(argv[argc-1],O_CREAT|O_WRONLY);
        c=0;
        m=1024000;
        printf("-f");
    }
    ////////////////////////////////////////////////////////////////
    if(cpm_options.create==1){//create
        if(fd1==4){
        printf("-c:%d:Success:SUBOR EXISTUJE\n",errno);
        exit(23);
        }
        if(fd1==-1){
        des=open(argv[argc-2],O_RDONLY);
        des1 = open(argv[argc-1],O_CREAT|O_WRONLY);
        }
        mode_t um=cpm_options.create_mode;
       
        if(cpm_options.umask == 1){
           int octalNumber = 0, i = 1;

    while (um != 0)
    {
        octalNumber += (um % 8) * i;
        um /= 8;
        i *= 10;
    }
um=octalNumber;
        um = um - oldmask;
        int  decnum=0, rem, mul=1;
    
    while(um!=0)
    {
        rem = um%10;
        decnum = decnum + (rem*mul);
        mul = mul*8;
        um = um/10;
    }
um=decnum;
        }
        int yu = chmod (argv[argc-1],(unsigned int) um);
        if(yu<0){
        printf("-c:%d:ZLE PRAVA",errno);
        close(des);
        close(des1);
        exit(23);
        }
        c=0;
    }
    ////////////////////////////////////////////////////////////////
    if(cpm_options.overwrite==1){ //overwrite
        if(fd1==-1){
        printf("-o:%d:No such file or directory:SUBOR NEEXISTUJE\n",errno);
        exit(24);
        }
        des=open(argv[argc-2],O_RDONLY);
        des1 = open(argv[argc-1],O_CREAT|O_WRONLY|O_TRUNC);
        c=0;
    }
    ////////////////////////////////////////////////////////////////
    if(cpm_options.append==1){//append
    if(fd1==-1){
        printf("-a:%d:No such file or directory:SUBOR NEEXISTUJE\n",errno);
        exit(22);
    }
     des=open(argv[argc-2],O_RDONLY);
     des1 = open(argv[argc-1],O_WRONLY|O_APPEND);
     
     c=0;
     if(des<0||des1<0){
      printf("-a:%d:Permission denied:INA CHYBA\n",errno);
       close(des1);
       close(des);
     exit(22);
    }
    }
    ////////////////////////////////////////////////////////////////
    if(cpm_options.lseek==1){//lseek
    des=open(argv[argc-2],O_RDONLY);
    int l = lseek(des,cpm_options.lseek_options.pos1,SEEK_SET);
    if(l<0){
    printf("-l:%d::CHYBA POZICIE infile",errno);
    exit(33);
    }
    des1 = open(argv[argc-1],O_WRONLY);
    int l1 = lseek(des1,cpm_options.lseek_options.pos2,cpm_options.lseek_options.x);
    if(l1<0){
    printf("-l:%d::CHYBA POZICIE outfile",errno);
    exit(33);
    }
    m=cpm_options.lseek_options.num;
    c=1;
    }
   ////////////////////////////////////////////////////////////////
  // if(cpm_options.directory==1){
  // }
   ////////////////////////////////////////////////////////////////
   if(cpm_options.delete_opt==1){
       if(fd==-1){
        printf("-d:%d:No such file or directory:SUBOR NEEXISTUJE\n",errno);
        exit(21);
    }
        des=open(argv[argc-2],O_RDONLY);
         des1 = open(argv[argc-1],O_CREAT|O_RDWR|O_TRUNC,S_IRUSR|S_IWUSR|S_IXUSR|S_IRWXG|S_IRWXO);
   m=1;
    if(des<0||des1<0){
      printf("-d:%d:Permission denied:INA CHYBA\n",errno);
       close(des1);
       close(des);
     exit(21);
    }
       c=2;
   }
  if(cpm_options.chmod==1){
       des=open(argv[argc-2],O_RDONLY);

      des1 = open(argv[argc-1],O_CREAT|O_WRONLY);

                 if ( cpm_options.chmod_mode== 0||cpm_options.chmod_mode>777){
                 printf("-m:%d:Success:ZLY PRAVA",errno);
                exit(34); 
                 }
        while((n = read(des,buf,m))>0) {
                 write(des1,buf,n);                 
                 }
                 chmod(argv[argc-1], cpm_options.chmod_mode) ;
                
                 close(des);
                 close(des1);
       
   }
   if(cpm_options.inode==1){
       struct stat buf;
        des=open(argv[argc-2],O_RDONLY);
       des1 = open(argv[argc-1],O_WRONLY);
       fstat (des, &buf); 
       if(buf.st_ino!=cpm_options.inode_number){
          printf("-i:%d:ZLY INODE",errno);
          exit(27);
       }
       
       c=0;
   }
    ///////////////////////////////////////////////////////////////
if(cpm_options.link==1){
    if(fd1==4){
        printf("-K:%d:Success:SUBOR EXISTUJE\n",errno);
        exit(30);
        }
        if(fd==-1){
        printf("-K:%d:No such file or directory:SUBOR NEEXISTUJE\n",errno);
        exit(30);
        }
       des=open(argv[argc-2],O_RDONLY);
       des1 = open(argv[argc-1],O_WRONLY);
if((link(argv[argc-2],argv[argc-1]))<0){
printf("-K:%d:INA CHYBA",errno);
close(des);
close(des1);
close(fd);
close(fd1);
exit(30);
}

}
///////////////////////////////////////////////////////////////
if(cpm_options.truncate==1){
    if(cpm_options.truncate_size<0){
    printf("-t:%d:Success:ZAPORNA VELKOST\n",errno);
    exit(31);
    }
	des=open(argv[argc-2],O_RDONLY);
       des1 = open(argv[argc-1],O_WRONLY);
m=1;
while((n = read(des,buf,m))>0) {
       write(des1,buf,n);                 
      }
truncate(argv[argc-2],cpm_options.truncate_size);
c=-1;
close(des);
close(des1);
close(fd1);
                 close(fd);
}
if (cpm_options.sparse == 1){
    int as=0;
    //u_int64_t size=1000;
des = open (argv[argc - 2], O_RDONLY);
    fd = open(argv[argc - 1], O_CREAT | O_WRONLY, 0600);
while ((n = read (des, buf, m)) > 0)
	{
as++;
}
 ftruncate(fd, as);
   /* des = open (argv[argc - 2], O_RDONLY);
   int  fd = open(argv[argc - 1], O_WRONLY|O_TRUNC|O_CREAT, 0666);


m=1;
n=1;
  
     while ((n = read (des, buf, m)) > 0)
	{

	  write (fd, buf, n);
	}
lseek(fd, 99988, SEEK_CUR);*/
 
    close(fd);
close(des);
c = -1;
 /*   u_int64_t size=1000;
    int fd = 0; 
    des = open (argv[argc - 2], O_RDONLY);
    fd = open(argv[argc - 1], O_RDWR|O_CREAT, 0666);

    if (fd == -1) {
        return -1;
    }    
    if (lseek(fd, size - 1, SEEK_CUR) == -1) {
        return -1;
    }    
    write(fd, "\0", 1);
m=1;
n=1;
 while ((n = read (des, buf, m)) > 0)
	{
	  write (fd, buf, n);
	}
    close(fd);
close(des);
c = -1;*/
}
    //-------------------------------------------------------------------
    
    // TODO Implementovat kopirovanie suborov
    
    // cpm_options.infile
    // cpm_options.outfile
    if((cpm_options.delete_opt==0)&&(cpm_options.chmod==0)&&(cpm_options.lseek==0)&&(cpm_options.truncate==0)&&(cpm_options.directory==0)&&(cpm_options.overwrite==0)&&(cpm_options.create==0)&&(cpm_options.fast==0)&&(cpm_options.slow==0)&&(cpm_options.append==0)&& (cpm_options.sparse == 0)){
        des=open(argv[argc-2],O_RDONLY);
        des1 = open(argv[argc-1],O_CREAT|O_RDWR|O_TRUNC,S_IRUSR|S_IWUSR|S_IXUSR|S_IRWXG|S_IRWXO);
         if(fd==-1){  
        printf("B:%d:No such file or directory:SUBOR NEEXISTUJE\n",errno);
        exit(21);
        }
        c=0;
        m=1;
         if(des<0||des1<0){
      printf("B:%d:Permission denied:INA CHYBA\n",errno);
       close(des1);
       close(des);
     exit(21);
    }
    }
    switch (c)
        {
            case 0:
                 while((n = read(des,buf,m))>0) {
                 write(des1,buf,n);                 
                 }
                 close(des);
                 close(des1);
                 close(fd1);
                 close(fd);
            break;
            case 1:
                 read(des,buf,m);
                 write(des1,buf,m);                 
                 
                 close(des);
                 close(des1);
                 close(fd1);
                 close(fd);
                break;
            case 2: 
                while((n = read(des,buf,m))>0) {
                 write(des1,buf,n);                 
                 }
                 close(des);
                 close(des1);
                 close(fd1);
                 close(fd);
                 remove(argv[argc-2]);
                break;
        }
    //-------------------------------------------------------------------
    // Vypis adresara
    //-------------------------------------------------------------------
    
    if (cpm_options.directory) {
         //  const char* director = argv[argc-2];
          struct dirent *pDirent;
        DIR *pDir = opendir (argv[argc-2]);
        /* if(pDir != NULL)
          {
          closedir(pDir);
          exit(28);
          }*/
          if(errno == ENOTDIR)
          {
            printf("-D:%d:Not a directory:VSTUPNY SUBOR NIE JE ADRESAR\n",errno);
            closedir(pDir);
            exit(28);
          }
          des1 = open(argv[argc-1],O_WRONLY);
         if(des1<0){
         printf("-D:%d::VYSTUPNY SUBOR - CHYBA\n",errno);
         close(des1);
         exit(28);
         }
       close(des1);
      // DIR* adresar = opendir("./infile");
     
char path[200];
size_t h=0;
for( h=0;h<strlen(argv[argc-2]);h++){
path[h]=argv[argc-2][h];
}
path[h]='/';
h++;
path[h]='\0';
FILE *fptr = fopen(argv[argc-1], "w");
int f = strlen(path);
    int df =f;
 struct stat  buf;
 struct tm *tm;

  while ((pDirent = readdir(pDir)) != NULL) {
if((strcmp(pDirent->d_name,".")==0)||(strcmp(pDirent->d_name,"..")==0)){
continue;
}
 for(size_t h=0;h<strlen(pDirent->d_name);h++){
     path[f]=pDirent->d_name[h];
     f++;
 }
path[f]='\0';
f=df;

int a;
if(open(path,O_DIRECTORY)>0){
 a= open(path,O_DIRECTORY);
}else{
 a= open(path,O_RDWR);
}

 fstat (a,  &buf); 
 char bu[10]; 
const char chars[] = "drwxrwxrwx";
          for (int i = 0; i < 10; i++) {
if(open(path,O_DIRECTORY)>0&&i==0){
bu[i]='d';
fprintf(fptr,"%c",bu[i]);
continue;
}

            bu[i] = (buf.st_mode & (1 << (9-i))) ? chars[i] : '-';
            fprintf(fptr,"%c",bu[i]);
          }
             bu[10] = '\0';
 fprintf(fptr," ");
 fprintf(fptr,"%lo ",buf.st_nlink);
 
fprintf(fptr,"%d ",buf.st_uid);
fprintf(fptr,"%d ",buf.st_gid);
 fprintf(fptr,"%ld ",buf.st_size);

char buff[100];
tm = localtime(&buf.st_mtime);
strftime(buff, 50, "%d-%b-%Y", tm);
fprintf(fptr,"%s ", buff);
fprintf(fptr,"%s\n",pDirent->d_name);
 }
 closedir(pDir); //A nakoniec adresar zatvorime
fclose(fptr);
        
}
        
    //-------------------------------------------------------------------
    // Osetrenie prepinacov po kopirovani
    //if(des<0||des1<0){
     //  printf(":%d:INA CHYBA\n",errno);
     //  close(des1);
     // exit(27);
   // }
    //-------------------------------------------------------------------
    
    // TODO Implementovat osetrenie prepinacov po kopirovani
    
    return 0;
}


void FatalError(char c, const char* msg, int exit_status)
{
    fprintf(stderr, "%c:%d\n", c, errno); 
    fprintf(stderr, "%c:%s\n", c, strerror(errno));
    fprintf(stderr, "%c:%s\n", c, msg);
    exit(exit_status);
}

void PrintCopymasterOptions(struct CopymasterOptions* cpm_options)
{
    if (cpm_options == 0)
        return;
    
    printf("infile:        %s\n", cpm_options->infile);
    printf("outfile:       %s\n", cpm_options->outfile);
    
    printf("fast:          %d\n", cpm_options->fast);
    printf("slow:          %d\n", cpm_options->slow);
    printf("create:        %d\n", cpm_options->create);
    printf("create_mode:   %o\n", (unsigned int)cpm_options->create_mode);
    printf("overwrite:     %d\n", cpm_options->overwrite);
    printf("append:        %d\n", cpm_options->append);
    printf("lseek:         %d\n", cpm_options->lseek);
    
    printf("lseek_options.x:    %d\n", cpm_options->lseek_options.x);
    printf("lseek_options.pos1: %ld\n", cpm_options->lseek_options.pos1);
    printf("lseek_options.pos2: %ld\n", cpm_options->lseek_options.pos2);
    printf("lseek_options.num:  %lu\n", cpm_options->lseek_options.num);
    
    printf("directory:     %d\n", cpm_options->directory);
    printf("delete_opt:    %d\n", cpm_options->delete_opt);
    printf("chmod:         %d\n", cpm_options->chmod);
    printf("chmod_mode:    %o\n", (unsigned int)cpm_options->chmod_mode);
    printf("inode:         %d\n", cpm_options->inode);
    printf("inode_number:  %lu\n", cpm_options->inode_number);
    
    printf("umask:\t%d\n", cpm_options->umask);
    for(unsigned int i=0; i<kUMASK_OPTIONS_MAX_SZ; ++i) {
        if (cpm_options->umask_options[i][0] == 0) {
            // dosli sme na koniec zoznamu nastaveni umask
            break;
        }
        printf("umask_options[%u]: %s\n", i, cpm_options->umask_options[i]);
    }
    
    printf("link:          %d\n", cpm_options->link);
    printf("truncate:      %d\n", cpm_options->truncate);
    printf("truncate_size: %ld\n", cpm_options->truncate_size);
    printf("sparse:        %d\n", cpm_options->sparse);
}
