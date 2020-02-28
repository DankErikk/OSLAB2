#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h> 
#include<sys/wait.h> 
  
#define MAX 100 // max number of letters to be supported 

int changeDirectory(char* args[]){
	// cd goes to home 
	if (args[1] == NULL) {
		chdir(getenv("HOME")); 
		return 1;
	}
    //cd goes to speciic place if exists
	else{ 
		if (chdir(args[1]) == -1) {
			printf("-bash: cd: %s No such file or directory\n", args[1]);
            return -1;
		}
	}
	return 0;
}

/**
* Method execute commands entered 
*/ 
int execCmds(char * args[]){
	
	// exit shell 
	if(strcmp(args[0],"exit") == 0) {
        exit(0);
    } 
	// 'cd' command to change directory
	else if (strcmp(args[0],"cd") == 0) changeDirectory(args);

return 1;
}

void loop(void) { 
    char line[MAX]; 
    char * tokens[MAX];
    char ** args;

    int numTokens;

    //reads parses, and executes commands
    while(1){
        display_prompt();

        // We empty the line buffer
		memset( line, '\0', MAX );

        fgets(line, MAX, stdin);

        // If nothing is written, the loop is executed again
		if((tokens[0] = strtok(line," \n\t")) == NULL) continue;

        numTokens = 1;
		while((tokens[numTokens] = strtok(NULL, " \n\t")) != NULL) numTokens++;
		
		execCmds(tokens);
    }
}

int main(int argc, char** argv) { 
    loop();

    return 0;
}

// Function to print Current Directory. 
void printDirectory() 
{ 
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); 
    printf("\n%s$ ", cwd); 
} 

void display_prompt(){
    // check if the ps1 variable is set
    // if its not set, print '$'
    if(!getenv("PS1")){
        printf("$");
    }
    else{
        printf("%s", getenv("PS1"));
    }
}