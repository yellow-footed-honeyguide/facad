#include "git_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool is_git_repository() {
    FILE* fp = popen("git rev-parse --is-inside-work-tree 2>/dev/null", "r");
    if (fp == NULL) {
        return false;
    }
    char buffer[128];
    bool result = fgets(buffer, sizeof(buffer), fp) != NULL && strcmp(buffer, "true\n") == 0;
    pclose(fp);
    return result;
}

static char parse_status(const char* status_str) {
    if (status_str[0] == '?' && status_str[1] == '?') return 'U';
    if (status_str[0] == '!' && status_str[1] == '!') return 'I';
    return status_str[0] != ' ' ? status_str[0] : status_str[1];
}

GitFileList get_git_status() {
    GitFileList list = {0};
    FILE* fp = popen("git status -s", "r");
    if (fp == NULL) {
        return list;
    }

    char line[MAX_FILENAME + 3];
    while (fgets(line, sizeof(line), fp) != NULL && list.count < MAX_FILES) {
        if (strlen(line) > 3) {
            GitFile file;
            file.status = parse_status(line);
            strncpy(file.filename, line + 3, MAX_FILENAME - 1);
            file.filename[strcspn(file.filename, "\n")] = 0;
            list.files[list.count++] = file;
        }
    }

    pclose(fp);
    return list;
}

char* get_current_branch() {
    FILE* fp = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
    if (fp == NULL) {
        return NULL;
    }

    char* branch = malloc(256);
    if (fgets(branch, 256, fp) == NULL) {
        free(branch);
        pclose(fp);
        return NULL;
    }

    pclose(fp);
    branch[strcspn(branch, "\n")] = 0;
    return branch;
}
