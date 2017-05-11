#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<fcntl.h>
struct metrics {
  int nbytes;
  int nlines;
  int type;
  char process1[1000];
  char process2[1000];
  char isascii[10];
  int pipenum;
};

int main(int argc, char **argv){
  pid_t id;
  int fds2[2];
  int fds3[2];
  int fds[2];
  pipe(fds2);
  pipe(fds3);
  pipe(fds);
  int i,j,k;
  int numpipes = 0;
  int children = 0;
  char *args[1000];
  j=0;
  long numbytes = 0;
  char buffer[512];
  FILE *f;
  char *arglist[1000][1000];
  int col;
  int count;
  char buf[100];
  struct metrics m;
  struct metrics pm;
  char bufft[100];
  char proc1[1000];
  char proc2[1000];

  for(i=0;i<argc;i++){
    if(strcmp(argv[i],"|")==0)
      numpipes++;
  }
  j=0;
  children = numpipes +1;
  for(i=1;i<argc;i++){
    col=0;
    while(strcmp(argv[i],"|")!=0){
      arglist[j][col] = malloc(strlen(argv[j])*4);
      arglist[j][col] = argv[i];
      i++;
      col++;
      if(i==argc){
	break;
      }
    }
    j++;
  }
  //printf("%s %s %s %s",arglist[0][0],arglist[0][1],arglist[1][0],arglist[1][1]);
   /* Fork first process */
    id = fork();

    if (id < 0) {
        printf("fork() for first process failed\n");
        exit(-1);
    }

    if (id == 0) {
      dup2(fds[1],1);
      close(fds[0]);
      close(fds[1]);
      close(fds2[0]);
      close(fds2[1]);
      close(fds3[0]);
      close(fds3[1]);

      if (execvp(arglist[0][0],arglist[0]) < 0) {
            write(2, "execlp() failed for prog1\n", 27);
            exit(-1);
        }

        /* We will never reach this point */
    }

    /* Fork middle process */

    id = fork();

    if (id < 0) {
        printf("fork() for second process failed\n");
        exit(-1);
    }

    if (id == 0) {
      dup2(fds[0],0);
      close(fds[0]);
      close(fds[1]);
      close(fds2[0]);
      //close(fds2[1]);
      close(fds3[0]);
      //close(fds3[1]);
      
      while ((count = read(0, buf, 1)) > 0) {
	strcat(bufft,buf);
	m.nbytes++;
	if(strcmp(buf,"\n")==0)
	m.nlines++;
       }
      //printf("final buf[] = %s\n", bufft);
      //printf("\nlength is %d",strlen(bufft));
      
      // append data to struct
      m.pipenum++;
      
      //m.process1 = (char*)arglist[0][0];
      //m.process2 = (char*)arglist[1][0];
      
       write(fds3[1],bufft,strlen(bufft));
       close(fds3[1]);
       write(fds2[1],(char*)&m,sizeof(m)); //was ,bufft,strlen(bufft)
       close(fds2[1]);
       exit(0);
      
    }
    /* making c2 */
    id = fork();
    if(id==0){
      dup2(fds3[0],0);
      close(fds3[0]);
      close(fds3[1]);
      close(fds[0]);
      close(fds[1]);
      close(fds2[0]);
      close(fds2[1]);
      if (execvp(arglist[1][0],arglist[1]) < 0) {
            write(2, "execvp() failed for prog2\n", 27);
            exit(-1);
        }
      
      
    }
    /* Need to close both ends of pipe in parent */
   
    close(fds[0]);
    close(fds[1]);
    //dup2(fds2[0],0);
    //close(fds2[0]);
    close(fds2[1]);
    close(fds3[0]);
    close(fds3[1]);
    /*while ((count = read(fds2[0], buf, 1)) > 0) {
	strcat(bufft,buf);
 
	}*/
    while ((count = read(fds2[0], &pm, sizeof(m))) > 0) {
	strcat(bufft,buf);
	
 
       }
    close(fds2[0]);
    //printf("parent final buf[] = %s\n", bufft);
    //printf("\nparent length is %d",strlen(bufft));
     printf("parent final bytes = %d\n", pm.nbytes);
    printf("\nparent nlines =  %d",pm.nlines);
    // printf("\n parent pipenum = %d ",m.pipenum);
    //printf("\n %s -> %s",m.process1,m.process2);
    id = wait(NULL);
    id = wait(NULL);
    id = wait(NULL);

  
    return 0;
  
}

