#include <stdio.h>
#include <windows.h>
#define _POSIX_SOURCE
#include <unistd.h>
#undef  _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#define B 1024
#define True 1

int main(){
	FILE * fp;
	int status;
	char path[MAX_PATH];
	char command[B];
	char cwd[256];
	char *cd;
	while(True){

	printf("~#: "); gets(command);

	if ((strstr(command, "cd")-command) == 0){
	cd = strtok(strchr(command, 32), " ");
		if (chdir(cd) != 0){
	    perror("chdir() error()");
	  }
	  else {
	    if (getcwd(cwd, sizeof(cwd)) == NULL){
	      perror("getcwd() error");
	    }
	    else{
	      printf("current working directory is: %s\n", cwd);
	    }
	  }
	}

	else{

	fp = popen (command, "r");
	if (fp == NULL){
	    printf("Handle error\n");
	    return 1;
	}


	while (fgets (path, MAX_PATH, fp)!= NULL){
	    printf ("%s", path);
	}


	status = pclose (fp);

	printf("status : %d\n", status);

	if (status == -1) {
	    printf("Erro relatado por pclose ()\n");
	    return 1;
	} else {

		printf("OK\n");
	    /* Use macros descritas em wait () para inspecionar 'status' em ordem
	       determinar o sucesso / falha do comando executado por popen () */
	    
	}
		
	}	
}

}