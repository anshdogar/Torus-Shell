#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
int main(){

    char input[MAX_INPUT];

    char* args[MAX_ARGS];

    while (1)
    {
        

        printf("(Torus)> ");

        fflush(stdout);

        if(fgets(input, MAX_INPUT, stdin) == NULL){ //preventive guard
            perror("Torus failed, please try again.\n");
            continue;
        }

        input[strcspn(input, "\n")] = '\0'; //takes input string and changes all instances of new line to end character

        if(strcmp(input, "exit") == 0){ //compares strings for values
            printf("Goodbye.\n");
            break;
        }
    char* token = strtok(input, " ");
    int i = 0;
    while (token != NULL && i < MAX_ARGS)
    {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    args[i] = NULL;

    pid_t pid = fork();

    if(pid == 0){
        execvp(args[0], args);
        perror("Torus failed, please try again.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid > 0){
        int status;
        waitpid(pid, &status, 0);
        printf("Existing status: %d\n", status);
    }

    else{
       perror("Torus failed, please try again.\n"); 
    }
    
}

    return 0;
}