#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h> 
#include<sys/wait.h> 
  
#define MAXLINE 1000 // max number of letters to be supported 

void loop(void) { 
    char line[MAXLINE]; 
    char ** args;
    int status; 

    //reads parses, and executes commands
    while(1){
        printDirectory();
        fgets(line, MAXLINE, stdin);
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
  