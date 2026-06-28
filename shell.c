#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

#define PURPLE "\033[38;2;220;92;255m"
#define RESET "\033[0m"
#define WHITE "\033[38;2;220;180;255m"


struct shortcut{
    char* before;
    char* after;
};

struct shortcutList{
    struct shortcut* shortcuts[MAX_INPUT];

    size_t size;
};

int contains(char* x, char* arr[], size_t n) {
    for(size_t i = 0; i < n; i++) {
        if(strcmp(arr[i], x) == 0)
            return 0;
        if(strcmp(arr[i], "") == 0) return 1;
    }
    return 1;
}

int containsShortcut(char* x, struct shortcutList* arr, size_t n) {
    for(size_t i = 0; i  < n; i++) {
        char* shortName = arr->shortcuts[i]->after;
        if(strcmp(shortName, x) == 0)
            return 0;
    }
    return 1;
}

char* findShortcut(char* after, struct shortcutList* scl, size_t n){
    for(size_t i = 0; i  < n; i++) {
        char* shortName = scl->shortcuts[i]->after;
        if(strcmp(shortName, after) == 0)
            return scl->shortcuts[i]->before;
    }
    return 0;
}

void addShortcut(char* old, char* new, struct shortcutList* shortcuts){

    struct shortcut* sc = (struct shortcut*)malloc(sizeof(struct shortcut));
    sc->before = strdup(old); //strdup: copies string into new heap space
    sc->after = strdup(new);
    shortcuts->shortcuts[shortcuts->size] = sc;
    shortcuts->size++;

}

char* deleteShortcut(char* toDelete, struct shortcutList* shortcuts){
    int isDeleted = 0;

    for(size_t i = 0; i  < shortcuts->size; i++) {
        if(isDeleted == 1){
            shortcuts->shortcuts[i - 1] = shortcuts->shortcuts[i];
        }
        else{
        char* shortName = shortcuts->shortcuts[i]->after;
        if(strcmp(shortName, toDelete) == 0){
            struct shortcut* s = shortcuts->shortcuts[i];
            free(s->before);
            free(s->after);
            free(s);
            isDeleted = 1;
        }
        }

    }

    shortcuts->size--;
    return toDelete;
}


int main(){

     char *execvp_commands[] = {
    /* file system */
    "ls", "cp", "mv", "rm", "mkdir", "rmdir", "touch", "ln",
    "chmod", "chown", "chgrp", "stat", "du", "df", "mount", "umount",
    "find", "locate", "pwd", "tree", "cd", "exit", "shortcut",

    /* text processing */
    "cat", "echo", "printf", "head", "tail", "grep", "sed", "awk",
    "cut", "sort", "uniq", "wc", "tr", "tee", "xargs", "diff", "patch",

    /* process management */
    "ps", "top", "kill", "killall", "pkill", "pgrep", "nice", "renice",
    "nohup", "wait", "sleep", "time",

    /* networking */
    "ping", "curl", "wget", "ssh", "scp", "rsync", "netstat", "ss",
    "ifconfig", "ip", "traceroute", "nslookup", "dig", "hostname",

    /* archives */
    "tar", "gzip", "gunzip", "zip", "unzip", "bzip2", "xz",

    /* user/system */
    "whoami", "who", "id", "su", "sudo", "passwd", "useradd", "userdel",
    "groupadd", "env", "printenv", "uname", "uptime", "date", "cal",

    /* editors / viewers */
    "vi", "vim", "nano", "less", "more", "man",

    /* shell / scripting */
    "sh", "bash", "python", "python3", "perl", "ruby", "node",
    "make", "gcc", "g++", "clang",

    /* misc */
    "clear", "tput", "stty", "test", "[", "true", "false",
    "read", "getopts", "expr"
};


    printf(PURPLE"\n\n\n________/\\\\\\\\\\\\\\\\\\__/\\\\\\_____\n_____/\\\\\\////////__\\///\\\\\\___\n___/\\\\\\/_____________\\//\\\\\\__\n__/\\\\\\________________\\//\\\\\\_\n_\\/\\\\\\_________________\\/\\\\\\_\n_\\//\\\\\\________________/\\\\\\__\n__\\///\\\\\\_____________/\\\\\\___\n____\\////\\\\\\\\\\\\\\\\\\__/\\\\\\/____\n_______\\/////////__\\///______\n\n\n"RESET);
    char input[MAX_INPUT];

    char* args[MAX_ARGS];

    char cwd[MAX_INPUT];

    char path[MAX_INPUT];

    getcwd(path, sizeof(path));

    struct shortcutList* shortcuts = malloc(sizeof(struct shortcutList));

    shortcuts->size = 0;

    size_t num_commands = sizeof(execvp_commands) / sizeof(execvp_commands[0]);


    FILE *readShortcuts = fopen("shortcuts.txt", "r");
    char buffer[255];
    char* shortcutSplit[2];
    while(fgets(buffer, 255, readShortcuts) != NULL){
        buffer[strcspn(buffer, "\n")] = '\0';

        char* shortcutInfo = strtok(buffer, " ");
        int i = 0;
        while (shortcutInfo != NULL && i < 2)
    {
        shortcutSplit[i] = shortcutInfo;
        shortcutInfo = strtok(NULL, " ");
        i++;
    }
        addShortcut(shortcutSplit[0], shortcutSplit[1], shortcuts);
    }
    fclose(readShortcuts);


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

        if(strcmp(input, "") == 0){
            continue;
        }

        int isEmpty = 0;

        for(int i = 0; i < strlen(input); i++){
            if(!(isspace(input[i]))){
                isEmpty = 1;
                break;
            }
            if(i == '\0') break;
        }
        if(isEmpty == 0){
            continue;
        }

        

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

    if(strcmp(args[0], "shortcut") == 0){
            char* bef = args[1];
            char* new = args[2];

        if(args[1] == NULL || args[2] == NULL){
            printf("shortcut missing arguments.\n ");
            continue;
        }

        if(strcmp(bef, "-D") == 0){
            if(containsShortcut(new, shortcuts, shortcuts->size) == 1){
                printf("Cannot delete shortcut which doesn't exist.\n");
                continue;
            }
            deleteShortcut(new, shortcuts);
            printf("Deletion of shortcut %s successfull.\n", new);
            continue;

        }
        else if(contains(bef, execvp_commands, num_commands) == 1){
            if(containsShortcut(bef, shortcuts, shortcuts->size) == 0){
                printf("You cannot create shortcuts of existing shortcuts. To make this word a shortcut for %s, use the command: shortcut %s %s\n", bef, findShortcut(bef, shortcuts, shortcuts->size), new);
            }
            else{
            printf("shortcut for invalid command.\n");
            }
            continue;
        }
        else if(contains(new, execvp_commands, num_commands) == 0){
            printf("cannot change existing command names.\n ");
            continue;
        }
        else{
            if(containsShortcut(new, shortcuts, shortcuts->size) == 0){
                printf("shortcut already exists.\n ");
                continue;
            }
            addShortcut(bef, new, shortcuts);
            printf("Addition of shortcut %s for %s successfull.\n ", new, bef);
            continue;
        }

    }

    if(containsShortcut(args[0], shortcuts, shortcuts->size) == 0){
        char* replacement = findShortcut(args[0], shortcuts, shortcuts->size);
        args[0] = replacement;
    }

    if(strcmp(args[0], "cd") == 0){
        if(args[1] == NULL){
            printf("cd missing arguments.\n ");
            continue;
        }
        else if(chdir(args[1]) != 0){
            perror("cd failed");
            continue;
        }
        else{
        continue;
        }
    }

    if(strcmp(args[0], "exit") == 0 && args[1] == NULL){ //compares strings for values
            printf("Goodbye.\n");
            break;
        }



    pid_t pid = fork();

    if(pid == 0){
        execvp(args[0], args);
        printf("Not valid command. \n");
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

    chdir(path);


    FILE *writeShortcuts = fopen("shortcuts.txt", "w");
    
    for(int i = 0; i < shortcuts->size; i++){
        fprintf(writeShortcuts, "%s %s\n", shortcuts->shortcuts[i]->before, shortcuts->shortcuts[i]->after);
    }

    fclose(writeShortcuts);

    return 0;
}