#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include "file_card.h"

void display_entries(FileCardInfo *entries, int num_entries, int term_width,
                     const char *current_dir, int show_path);

#endif // DISPLAY_UTILS_H
