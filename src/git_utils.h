#ifndef GIT_UTILS_H
#define GIT_UTILS_H

#include <stdbool.h>

#define MAX_FILENAME 1024
#define MAX_FILES 1000

typedef struct {
    char filename[MAX_FILENAME];
    char status;
} GitFile;

typedef struct {
    GitFile files[MAX_FILES];
    int count;
} GitFileList;

bool is_git_repository(void);
GitFileList get_git_status(void);
char* get_current_branch(void);

#endif // GIT_UTILS_H
