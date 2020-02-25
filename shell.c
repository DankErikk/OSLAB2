#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX 50 

int main(){

    while(1){
        
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