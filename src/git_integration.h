#ifndef GIT_INTEGRATION_H
#define GIT_INTEGRATION_H

#include "file_card.h"
#include "git_utils.h"

void integrate_git_status(FileCardInfo** entries, int* num_entries, const char* current_dir);

#endif // GIT_INTEGRATION_H
