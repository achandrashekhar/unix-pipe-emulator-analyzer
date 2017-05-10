#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char **argv){
  pid_t id;
  int fildes[2];
  int fildes2[2];
  pipe(fildes);
  pipe(fildes2);
  int i,j,k;
  int numpipes = 0;
  int children = 1;
  char *args[1000];
  j=0;
  long numbytes = 0;
  char buffer[512];
  FILE *f;

  for(i=0;i<argc;i++){
    if(strcmp(argv[i],"|")==0)
      children++;
  }
  //children = numpipes +1;
  printf("\nChildren = %d",children);
  k = 0;
  j = 0;
  int length = 0;
  int p;
  //  for(i=0;i<children;i++){
    k=0;
  while(j<argc)
  {
    j++;
    if(argv[j]!=NULL)
     {
      if((strcmp(argv[j],"|")!=0)){
	args[k] = argv[j];
	k++;
	length++;
      }
      else {
	break;
      }
     }
  }
  args[k]=NULL;

  id = fork();
  if(id==0){
    printf("inside child");
    close(1);
    dup(fildes[1]);
    close(fildes[1]);
    close(fildes[0]);
    fprintf(f,"%s",args[0]);
    if(execvp(args[0],args)<0){
      exit(-1);
    }
  }
  wait(NULL);
  // the for loop for children}
  
}

