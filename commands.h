#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
 

typedef struct {
    char  **items;
    size_t  count;
    size_t  cap;
} StrList;
 
static void sl_init(StrList *l) {
    l->count = 0;
    l->cap   = 256;
    l->items = malloc(l->cap * sizeof *l->items);
    if (!l->items) { perror("malloc"); exit(EXIT_FAILURE); }
}
 
static void sl_push(StrList *l, const char *s) {
    if (l->count == l->cap) {
        l->cap *= 2;
        char **g = realloc(l->items, l->cap * sizeof *l->items);
        if (!g) { perror("realloc"); exit(EXIT_FAILURE); }
        l->items = g;
    }
    l->items[l->count] = strdup(s);
    if (!l->items[l->count]) { perror("strdup"); exit(EXIT_FAILURE); }
    l->count++;
}
 
static int cmp_str(const void *a, const void *b) {
    return strcmp(*(const char *const *)a, *(const char *const *)b);
}
 
static void scan_dir(const char *dir, StrList *out) {
    DIR *d = opendir(dir);
    if (!d) return;                       /* missing/unreadable dir: skip */
 
    struct dirent *e;
    char path[4096];
    while ((e = readdir(d)) != NULL) {
        if (e->d_name[0] == '.') continue;
        int n = snprintf(path, sizeof path, "%s/%s", dir, e->d_name);
        if (n < 0 || (size_t)n >= sizeof path) continue;
 
        struct stat st;
        if (stat(path, &st) != 0)     continue;   /* stat resolves symlinks */
        if (!S_ISREG(st.st_mode))     continue;   /* must be a regular file */
        if (access(path, X_OK) != 0)  continue;   /* must be executable     */
 
        sl_push(out, e->d_name);
    }
    closedir(d);
}
 

char **get_commands(void) {
    const char *path_env = getenv("PATH");
    if (!path_env) return NULL;
 
    char *copy = strdup(path_env);
    if (!copy) { perror("strdup"); exit(EXIT_FAILURE); }
 
    StrList list;
    sl_init(&list);
 
    for (char *dir = strtok(copy, ":"); dir; dir = strtok(NULL, ":"))
        scan_dir(dir, &list);
    free(copy);
 
    qsort(list.items, list.count, sizeof *list.items, cmp_str);
 
    /* compact in place, dropping duplicates */
    size_t w = 0;
    for (size_t r = 0; r < list.count; r++) {
        if (r == 0 || strcmp(list.items[r], list.items[r - 1]) != 0)
            list.items[w++] = list.items[r];
        else
            free(list.items[r]);          /* discard duplicate copy */
    }
    list.items[w++] = "cd";
 
    /* NULL-terminate (grow by one slot) */
    char **result = realloc(list.items, (w + 1) * sizeof *result);
    if (!result) { perror("realloc"); exit(EXIT_FAILURE); }
    result[w] = NULL;
    return result;
}
 
void free_commands(char **cmds) {
    if (!cmds) return;
    for (size_t i = 0; cmds[i]; i++) free(cmds[i]);
    free(cmds);
}