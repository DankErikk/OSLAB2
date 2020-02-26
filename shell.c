#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAX 50 

int main(){
    printf("RESULT IS \n");
    char* userName;
    getlogin_r(userName, sizeof(userName));
    printf("%s\n",userName);

    while(1){
        printf("ITERATING THROUGH WHILE LOOP\n");
        //printf("ps1 var is \u@\h:\W> \n");
        char input[MAX];
        char checkExit[MAX] = "exit";

        int result;

        //input = malloc (sizeof (char) * MAX);
        fgets(input, MAX, stdin);
        
        result = strcmp(input, checkExit);

        //printf("strcmp(str1, str3) = %s %d\n", input, result);

        if (strcmp(input, checkExit) == 10){
            printf("Duck you = ");
            break;
            exit(0);
        }
    }
    return 0;
}

char *readInput() {

	char *input;
	input = malloc (sizeof (char) * MAX);
	fgets(input, MAX, stdin);

	return input;

}