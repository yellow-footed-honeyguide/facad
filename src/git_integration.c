#include "git_integration.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>

void integrate_git_status(FileCardInfo** entries, int* num_entries, const char* current_dir) {
    if (!is_git_repository()) {
        return;
    }

    GitFileList git_status = get_git_status();

    char* base_dir = strdup(current_dir);
    char* dir_name = basename(base_dir);

    for (int i = 0; i < *num_entries; i++) {
        (*entries)[i].git_status[0] = '\0';
        (*entries)[i].git_status[1] = '\0';

        char relative_path[MAX_FILENAME];
        snprintf(relative_path, sizeof(relative_path), "%s/%s", dir_name, (*entries)[i].name);

        for (int j = 0; j < git_status.count; j++) {
            if (strcmp(relative_path, git_status.files[j].filename) == 0 ||
                strcmp((*entries)[i].name, git_status.files[j].filename) == 0) {
                (*entries)[i].git_status[0] = git_status.files[j].status;
                (*entries)[i].git_status[1] = '\0';
                break;
            }
        }
    }

    free(base_dir);
}

