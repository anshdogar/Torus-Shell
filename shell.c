#include <stdio.h>
#include <string.h>

#define MAX_INPUT 1024
int main(){
    char input[MAX_INPUT];

    while (1)
    {
        

        printf("(Torus)> ");

        fflush(stdout);

        if(fgets(input, MAX_INPUT, stdin) == NULL){
            perror("Torus failed, please try again");
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        if(strcmp(input, "exit") == 0){
            printf("Goodbye.\n");
            break;
        }
    printf("%s\n", input);

    }
    

    return 0;
}