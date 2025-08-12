#ifndef GIT_INTEGRATION_H
#define GIT_INTEGRATION_H

#include "file_card.h"
#include "git_utils.h"

#define MAX_PATH 4096
#define MAX_MODIFIED_DIRS 1000  // Maximum number of modified directories to track

void integrate_git_status(FileCardInfo** entries, int* num_entries, const char* current_dir);

#endif // GIT_INTEGRATION_H