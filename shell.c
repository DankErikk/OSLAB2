#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h> 
#include<sys/wait.h> 
  
#define MAX 100 // max number of letters to be supported 

int changeDirectory(char* args[]){

	// cd goes to home when no directory is written
	if (args[1] == NULL) {
		chdir(getenv("HOME")); 
		return 0;
	}
    //cd goes to speciic place if exists
	else{ 
		if (chdir(args[1]) == -1) {
			printf("-bash: cd: %s No such file or directory\n", args[1]);
            return -1;
		}
        else{
            return 0;
        }
	}
	return 0;
}

void runCommand(char** args){
    int returnStatus = execvp(*args, args);
    if(returnStatus<0){
        printf("%s: command not found\n", *args);
        exit(-1);
    }
    else{
        exit(0);
    }
}
/**
* Method execute commands entered 
*/ 
int execCmds(char * args[]){

    pid_t pid;
    int exitFlag;
    int status;

    if((pid= fork()) < 0){
        printf("Error forking!\n");
        return -1;
    }
    else if(pid==0){ // child

        printf("CHILD PID: %d\n", getpid());

        // 'cd' command to change directory
        if (strcmp(args[0],"cd") == 0) {
            changeDirectory(args);
        }
        else{
            runCommand(args);
        } 
        printf("END OF ELSE STATEMENTS THIS SHOULD NOT BE PRINTED");
    }
    else{ // parent

        // makes the parent wait until the child is done
        printf("PARENT PID: %d\n", getpid());
        int status;
        if ( waitpid(pid, &status, 0) == -1 ) {
            printf("waitpid failed\n");
            return EXIT_FAILURE;
        }
        if (WIFEXITED(status)) {
            printf("Program terminated with exit code %d\n", WEXITSTATUS(status));
        }
        wait(NULL);
    }

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

void loop(void) {
    // string for user input
    char line[MAX]; 
    //  max amount of arguments
    char * tokens[MAX];
    // arg count
    char ** argCount;

    int numTokens;
    int returnStatus;

    //reads parses, and executes commands
    while(1){
        display_prompt();
        // resets the buffer and get it
		memset( line, '\0', MAX );
        fgets(line, MAX, stdin);

        // check if exit was typed

        // tokenizes the string
		if((tokens[0] = strtok(line," \n\t")) == NULL) continue;

        // count the number of tokens
        numTokens = 1;
		while((tokens[numTokens] = strtok(NULL, " \n\t")) != NULL) numTokens++;
        printf("numTokens is %d\n", numTokens);
        
        // check if exit was typed
        if(strcmp(tokens[0], "exit") == 0){
            printf("Exiting...\n");
            exit(0);
        }
        // check if user typed in cd
        if(strcmp(tokens[0], "cd") == 0){
            int status = changeDirectory(tokens);
            if (status == -1){
                printf("Program terminated with exit code %d\n", status);
            }
            printf("exit status was %d\n", status);
            continue;
        }
        // check if there is an ampersand at the end of the command
        // this will indicate it to run in the background
        if(strcmp(tokens[numTokens-1], "&") == 0 ){
            // move terminator one value back
            tokens[numTokens-1] = tokens[numTokens];

            pid_t pid;
            int exitFlag;
            int status;
            // run the command as a child process
            if((pid= fork()) < 0){
                printf("Error forking!\n");
                continue;
            }
            else if(pid==0){ // child

                //printf("CHILD PID: %d\n", getpid());

                // 'cd' command to change directory
                if (strcmp(tokens[0],"cd") == 0) {
                    changeDirectory(tokens);
                    exit(0);
                }
                else{
                    runCommand(tokens);
                    exit(0);
                } 
                //printf("END OF ELSE STATEMENTS THIS SHOULD NOT BE PRINTED");
            }
            else{ // parent

                // makes the parent wait until the child is done
                //printf("PARENT PID: %d\n", getpid());
                int status;
                if ( waitpid(pid, &status, 0) == -1 ) {
                    printf("waitpid failed\n");
                    continue;
                }
                if (WIFEXITED(status)) {
                    printf("Program terminated with exit code %d\n", WEXITSTATUS(status));
                }
                // remove the wait and have the loop continue so that the command can run in the background
                continue;
            }

        }
        

		execCmds(tokens);
    }
}



int main(int argc, char** argv) {
    loop();
    return 0;
}


