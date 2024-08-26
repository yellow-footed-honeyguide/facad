#ifndef FILE_ENTRY_H
#define FILE_ENTRY_H

typedef struct {
    char *name;
    char *emoji;
    int is_directory;
    int is_hidden;
} FileEntry;

int create_file_entry(FileEntry *entry, const char *path);
void free_file_entry(FileEntry *entry);
int compare_file_entries(const void *a, const void *b);

#endif  // FILE_ENTRY_H
