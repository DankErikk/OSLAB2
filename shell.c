#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include <sys/stat.h>
#include <fcntl.h>
#define MAX 100 // max number of letters to be supported 

/**
* Method used to manage the "|" character. This method will seperate the 
* commands before and after the "|" and a pipe will be used to store the 
* process of the commands before the "|" so that the command after the "|"
* can be executed on the first command.
*/ 
void execPipe(char * tokens[]){
	int processOne[2]; 
	int processTwo[2];
	
	int numCmnds = 0;
	
	char *cmnd[256];
	
	pid_t pid;
	
	int end = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	
	// Calculate the number of commands 
	while (tokens[l] != NULL){
		if (strcmp(tokens[l],"|") == 0){
			numCmnds++;
		}
		l++;
	}
	numCmnds++;
	
    // Will account for the commands before and after the "|",
    // there will be an input and output to be executed after 
    // processing is done 
	while (tokens[j] != NULL && end != 1){
		k = 0;
		
        //Stores commands
		while (strcmp(tokens[j],"|") != 0){
			cmnd[k] = tokens[j];
			j++;	
			if (tokens[j] == NULL){
				end = 1;
				k++;
				break;
			}
			k++;
		}

        //Needed to find the end 
		cmnd[k] = NULL;
		j++;		
		
        // Helps keep track of pipes inputs and output so that it can be connected 
		if (i % 2 != 0){
			pipe(processOne); 
		}else{
			pipe(processTwo); 
		}
		
		pid=fork();
		
		if(pid==-1){			
			if (i != numCmnds - 1){
				if (i % 2 != 0){
					close(processOne[1]); 
				}else{
					close(processTwo[1]); 
				} 
			}			
			printf("Child Process Failed\n");
			return;
		}
		if(pid==0){
			if (i == 0){
				dup2(processTwo[1], STDOUT_FILENO);
			}

            // On the last command, replace the input but sabe the output
			else if (i == numCmnds - 1){
				if (numCmnds % 2 != 0){ 
					dup2(processOne[0],STDIN_FILENO);
				}else{ 
					dup2(processTwo[0],STDIN_FILENO);
				}

            // Uses two pipes for input and output, respectively, saves the position
			}else{ 
				if (i % 2 != 0){
					dup2(processTwo[0],STDIN_FILENO); 
					dup2(processOne[1],STDOUT_FILENO);
				}else{ 
					dup2(processOne[0],STDIN_FILENO); 
					dup2(processTwo[1],STDOUT_FILENO);					
				} 
			}
            
			if (execvp(cmnd[0],cmnd) == -1){
				kill(getpid(),SIGTERM);
			}		
		}
				
		// closing processes
		if (i == 0){
			close(processTwo[1]);
		}
		else if (i == numCmnds - 1){
			if (numCmnds % 2 != 0){					
				close(processOne[0]);
			}else{					
				close(processTwo[0]);
			}
		}else{
			if (i % 2 != 0){					
				close(processTwo[0]);
				close(processOne[1]);
			}else{					
				close(processOne[0]);
				close(processTwo[1]);
			}
		}
				
		waitpid(pid,NULL,0);
				
		i++;	
	}
}

/** 
 * Method used to change directory 
 */
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

/**
 * Helper function to account for commands
 */
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

        //printf("CHILD PID: %d\n", getpid());

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
        // printf("PARENT PID: %d\n", getpid());
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

/**
 *  Displays the "$" unless otherwise stated
 */ 
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

/**
 *  Infintite loop to take in user commands
 */ 
void loop(void) {
    // string for user input
    char line[MAX]; 

    //  max amount of arguments
    char * tokens[MAX];
    // arg count
    char ** argCount;

    int numTokens;
    int returnStatus;
    int nameOfFile;
    int standardOut;

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
        //printf("numTokens is %d\n", numTokens);
        
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
            //printf("exit status was %d\n", status);
            continue;
        }

        //Account for the ">" command, makes a new file 
        //(Hardcoded for the moment, so the > has to be the second token and the name
        //must be on the third token)
        if ( (numTokens >= 2) && (strcmp(tokens[1],">") == 0)){
                //Determines the file name 
				nameOfFile = open(tokens[2], O_CREAT | O_TRUNC | O_WRONLY, 0600); 
				//Needed to swap 
				standardOut = dup(STDOUT_FILENO); 	

				dup2(nameOfFile, STDOUT_FILENO); 
				close(nameOfFile);
                //What will be in the file name 
				printf("%d\n", STDOUT_FILENO);
				dup2(standardOut, STDOUT_FILENO);
			}

        // Takes care of "|"
        if (numTokens >= 2 && strcmp(tokens[1],"|") == 0){
            execPipe(tokens);
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