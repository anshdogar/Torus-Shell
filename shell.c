#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

#define PURPLE "\033[38;2;220;92;255m"
#define RESET "\033[0m"
#define WHITE "\033[38;2;220;180;255m"
int main(){


    printf(PURPLE"\n\n\n________/\\\\\\\\\\\\\\\\\\__/\\\\\\_____\n_____/\\\\\\////////__\\///\\\\\\___\n___/\\\\\\/_____________\\//\\\\\\__\n__/\\\\\\________________\\//\\\\\\_\n_\\/\\\\\\_________________\\/\\\\\\_\n_\\//\\\\\\________________/\\\\\\__\n__\\///\\\\\\_____________/\\\\\\___\n____\\////\\\\\\\\\\\\\\\\\\__/\\\\\\/____\n_______\\/////////__\\///______\n\n\n"RESET);
    char input[MAX_INPUT];

    char* args[MAX_ARGS];

    char* cwd[MAX_INPUT];


    while (1)
    {
        
        if(getcwd(cwd, sizeof(cwd)) != NULL){
            printf(PURPLE"(Torus %s)> "WHITE, cwd);
        }
        else{
        printf(PURPLE"(Torus)> "WHITE);
        }

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
        args[i++] = token;
        token = strtok(NULL, " ");
    }

    

    args[i] = NULL;

    if(strcmp(args[0], "cd") == 0){
        if(args[1] == NULL){
            perror("cd missing arguments. ");
        }
        else if(chdir(args[1]) != 0){
            perror("cd failed");
        }
        else{
        continue;
        }
    }

    pid_t pid = fork();

    if(pid == 0){
        execvp(args[0], args);
        perror("Not valid command. ");
        exit(EXIT_FAILURE);
    }
    else if(pid > 0){
        int status;
        waitpid(pid, &status, 0);
        // printf("Existing status: %d\n", status);
    }

    else{
       perror("Torus failed, please try again.\n"); 
    }
    
}

    return 0;
}