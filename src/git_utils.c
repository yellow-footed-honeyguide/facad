#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "git_utils.h"

bool is_git_repository(const char* path) {
    char command[1024];
    snprintf(command, sizeof(command), "git -C \"%s\" rev-parse --is-inside-work-tree 2>/dev/null", path);
    
    FILE* fp = popen(command, "r");
    if (!fp) return false;
    
    char buffer[128];
    bool result = fgets(buffer, sizeof(buffer), fp) && strcmp(buffer, "true\n") == 0;
    pclose(fp);
    return result;
}

char parse_status(const char* status_str) {
    if (status_str[0] == '?' && status_str[1] == '?') return '?';
    return status_str[0] != ' ' ? status_str[0] : status_str[1];
}

GitFileList get_git_status(const char* path) {
    GitFileList list = {0};
    char command[1024];
    snprintf(command, sizeof(command), "git -C \"%s\" status -s", path);
    
    FILE* fp = popen(command, "r");
    if (!fp) return list;

    char line[MAX_FILENAME + 3];
    while (fgets(line, sizeof(line), fp) && list.count < MAX_FILES) {
        if (strlen(line) > 3) {
            GitFile file;
            file.status = parse_status(line);
            strncpy(file.filename, line + 3, MAX_FILENAME - 1);
            file.filename[strcspn(file.filename, "\n")] = '\0';
            list.files[list.count++] = file;
        }
    }
    pclose(fp);
    return list;
}

char* get_current_branch(const char* path) {
    char command[1024];
    snprintf(command, sizeof(command), "git -C \"%s\" rev-parse --abbrev-ref HEAD 2>/dev/null", path);
    
    FILE* fp = popen(command, "r");
    if (!fp) return NULL;

    char* branch = malloc(256);
    if (!fgets(branch, 256, fp)) {
        free(branch);
        pclose(fp);
        return NULL;
    }
    pclose(fp);
    branch[strcspn(branch, "\n")] = '\0';
    return branch;
}
