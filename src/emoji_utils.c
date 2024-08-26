// (f)function, (t)type, (m)macros, (s)structure
#include <ctype.h>     // isprint (f), isspace (f)
#include <stdio.h>     // printf (f), fprintf (f)
#include <stdlib.h>    // malloc (f), free (f)
#include <string.h>    // strcpy (f), strcmp (f), strrchr (f)
#include <sys/stat.h>  // struct stat (s), lstat (f), S_ISLNK (m), S_ISDIR (m)

#include "emoji_utils.h"  // get_emoji (f), is_executable (f), is_text_file (f)

#define MAX_PATH 4096  // Maximum path length (m)

char *get_emoji(const char *path) {  // Function to get emoji for a file path
    struct stat path_stat;           // Structure to hold file stats
    char *emoji =
        malloc(5 * sizeof(char));  // Allocate memory for emoji (max 4 bytes + null terminator)
    if (emoji == NULL) {           // Check if memory allocation failed
        return NULL;               // Return NULL if allocation failed
    }

    if (lstat(path, &path_stat) != 0) {  // Get file stats, using lstat to handle symlinks
        strcpy(emoji, "❓");             // Use question mark emoji if stats can't be retrieved
        return emoji;                    // Return the emoji
    }

    if (S_ISLNK(path_stat.st_mode)) {      // Check if the file is a symbolic link
        if (S_ISDIR(path_stat.st_mode)) {  // Check if the link points to a directory
            strcpy(emoji, "🔗📁");         // Use link + folder emoji for directory links
        } else {
            strcpy(emoji, "🔗");  // Use link emoji for other links
        }
        return emoji;  // Return the emoji
    }

    if (S_ISDIR(path_stat.st_mode)) {  // Check if the file is a directory
        strcpy(emoji, "📁");           // Use folder emoji for directories
        return emoji;                  // Return the emoji
    }

    char *extension = strrchr(path, '.');  // Find the last occurrence of '.' in the path
    if (extension != NULL) {               // If an extension was found
        extension++;                       // Move past the '.' to the actual extension
    }

    if (extension != NULL) {  // If there's a valid extension
        // Series of if-else statements to match file extensions with emojis
        if (strcmp(extension, "md") == 0)
            strcpy(emoji, "📑");  // Markdown files
        else if (strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0 ||
                 strcmp(extension, "png") == 0 || strcmp(extension, "gif") == 0 ||
                 strcmp(extension, "bmp") == 0 || strcmp(extension, "svg") == 0 ||
                 strcmp(extension, "webp") == 0)
            strcpy(emoji, "📸");  // Image files
        else if (strcmp(extension, "mp4") == 0 || strcmp(extension, "avi") == 0 ||
                 strcmp(extension, "mkv") == 0 || strcmp(extension, "mov") == 0 ||
                 strcmp(extension, "flv") == 0 || strcmp(extension, "wmv") == 0 ||
                 strcmp(extension, "webm") == 0)
            strcpy(emoji, "🎬");  // Video files
        else if (strcmp(extension, "mp3") == 0 || strcmp(extension, "wav") == 0 ||
                 strcmp(extension, "ogg") == 0 || strcmp(extension, "flac") == 0 ||
                 strcmp(extension, "m4a") == 0 || strcmp(extension, "aac") == 0)
            strcpy(emoji, "🎧");  // Audio files
        else if (strcmp(extension, "zip") == 0 || strcmp(extension, "tar") == 0 ||
                 strcmp(extension, "gz") == 0 || strcmp(extension, "bz2") == 0 ||
                 strcmp(extension, "xz") == 0 || strcmp(extension, "7z") == 0 ||
                 strcmp(extension, "rar") == 0)
            strcpy(emoji, "📦");  // Archive files
        else if (strcmp(extension, "deb") == 0 || strcmp(extension, "rpm") == 0)
            strcpy(emoji, "📥");  // Package files
        else if (strcmp(extension, "py") == 0 || strcmp(extension, "sh") == 0 ||
                 strcmp(extension, "js") == 0 || strcmp(extension, "html") == 0 ||
                 strcmp(extension, "css") == 0 || strcmp(extension, "cpp") == 0 ||
                 strcmp(extension, "c") == 0 || strcmp(extension, "java") == 0 ||
                 strcmp(extension, "go") == 0 || strcmp(extension, "rb") == 0 ||
                 strcmp(extension, "rs") == 0 || strcmp(extension, "php") == 0 ||
                 strcmp(extension, "h") == 0 || strcmp(extension, "hpp") == 0)
            strcpy(emoji, "💻");  // Code files
        else if (strcmp(extension, "o") == 0)
            strcpy(emoji, "🧩");  // Object files
        else if (strcmp(extension, "txt") == 0 || strcmp(extension, "rst") == 0 ||
                 strcmp(extension, "log") == 0)
            strcpy(emoji, "📝");  // Text files
        else if (strcmp(extension, "ttf") == 0 || strcmp(extension, "otf") == 0 ||
                 strcmp(extension, "woff") == 0 || strcmp(extension, "woff2") == 0)
            strcpy(emoji, "🔤");  // Font files
        else if (strcmp(extension, "pdf") == 0 || strcmp(extension, "djvu") == 0 ||
                 strcmp(extension, "epub") == 0)
            strcpy(emoji, "📚");  // E-book files
        else if (strcmp(extension, "pem") == 0 || strcmp(extension, "crt") == 0 ||
                 strcmp(extension, "key") == 0 || strcmp(extension, "pub") == 0 ||
                 strcmp(extension, "p12") == 0)
            strcpy(emoji, "🔑");  // Certificate files
        else if (strcmp(extension, "csv") == 0)
            strcpy(emoji, "📊");  // CSV files
        else if (strcmp(extension, "torrent") == 0)
            strcpy(emoji, "🌊");  // Torrent files
        else if (strcmp(extension, "iso") == 0 || strcmp(extension, "img") == 0)
            strcpy(emoji, "💽");  // Disk image files
        else if (strcmp(extension, "qcow") == 0 || strcmp(extension, "qcow2") == 0)
            strcpy(emoji, "🐮");  // QEMU disk image files
        else if (strcmp(extension, "vv") == 0)
            strcpy(emoji, "🕹️");  // Specific file type (unknown)
        else if (strcmp(extension, "doc") == 0 || strcmp(extension, "docx") == 0 ||
                 strcmp(extension, "odt") == 0 || strcmp(extension, "rtf") == 0 ||
                 strcmp(extension, "xls") == 0 || strcmp(extension, "xlsx") == 0 ||
                 strcmp(extension, "ods") == 0 || strcmp(extension, "ppt") == 0 ||
                 strcmp(extension, "pptx") == 0 || strcmp(extension, "odp") == 0)
            strcpy(emoji, "📄");  // Document files
        else if (strcmp(extension, "conf") == 0 || strcmp(extension, "config") == 0 ||
                 strcmp(extension, "toml") == 0 || strcmp(extension, "cfg") == 0 ||
                 strcmp(extension, "yaml") == 0 || strcmp(extension, "yml") == 0 ||
                 strcmp(extension, "json") == 0 || strcmp(extension, "ini") == 0)
            strcpy(emoji, "⚙️");                // Configuration files
        else {                                 // If the extension doesn't match any of the above
            if (path[0] == '.') {              // Check if it's a hidden file
                strcpy(emoji, "⚙️");            // Use gear emoji for hidden files
            } else if (is_executable(path)) {  // Check if the file is executable
                strcpy(emoji, "💾");           // Use floppy disk emoji for executable files
            } else if (is_text_file(path)) {   // Check if it's a text file
                strcpy(emoji, "📝");           // Use memo emoji for text files
            } else {
                strcpy(emoji, "❓");  // Use question mark for unknown file types
            }
        }
    } else {                               // If there's no file extension
        if (path[0] == '.') {              // Check if it's a hidden file
            strcpy(emoji, "⚙️");            // Use gear emoji for hidden files
        } else if (is_executable(path)) {  // Check if the file is executable
            strcpy(emoji, "💾");           // Use floppy disk emoji for executable files
        } else if (is_text_file(path)) {   // Check if it's a text file
            strcpy(emoji, "📝");           // Use memo emoji for text files
        } else {
            strcpy(emoji, "❓");  // Use question mark for unknown file types
        }
    }

    return emoji;  // Return the determined emoji
}

int is_executable(const char *path) {
    struct stat st;             // Structure to hold file stats
    if (stat(path, &st) < 0) {  // Get file stats, return 0 if fails
        return 0;
    }
    return (st.st_mode & S_IXUSR) != 0;  // Check if user execute permission is set
}

int is_text_file(const char *path) {
    FILE *file = fopen(path, "rb");  // Open file in binary read mode
    if (file == NULL) {              // Return 0 if file can't be opened
        return 0;
    }
    unsigned char buffer[1024];                                  // Buffer to read file contents
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), file);  // Read up to 1024 bytes
    fclose(file);                                                // Close the file
    if (bytes_read == 0) {  // If file is empty, consider it as text
        return 1;
    }
    for (size_t i = 0; i < bytes_read; i++) {              // Check each byte
        if (!isprint(buffer[i]) && !isspace(buffer[i])) {  // If byte is not printable or whitespace
            return 0;                                      // Consider it as non-text file
        }
    }
    return 1;  // If all bytes are printable or whitespace, it's a text file
}

char *get_dev_emoji(const char *path) {
    char *filename = strrchr(path, '/');  // Find last occurrence of '/' in path
    if (filename == NULL) {               // If no '/' found, use entire path
        filename = (char *)path;
    } else {
        filename++;  // Move past the '/'
    }
    char *emoji =
        malloc(5 * sizeof(char));  // Allocate memory for emoji (max 4 bytes + null terminator)
    if (emoji == NULL) {           // Check if memory allocation failed
        return NULL;
    }
    if (strcmp(filename, "null") == 0 || strcmp(filename, "zero") == 0) {
        strcpy(emoji, "⓿");  // Use '⓿' for null or zero devices
    } else if (strcmp(filename, "random") == 0 || strcmp(filename, "urandom") == 0) {
        strcpy(emoji, "🎲");  // Use '🎲' for random devices
    } else if (strncmp(filename, "tty", 3) == 0) {
        strcpy(emoji, "🖥️");  // Use '🖥️' for TTY devices
    } else if (strncmp(filename, "sd", 2) == 0) {
        strcpy(emoji, "💽");  // Use '💽' for SD card devices
    } else if (strncmp(filename, "loop", 4) == 0) {
        strcpy(emoji, "🔁");  // Use '🔁' for loop devices
    } else if (strncmp(filename, "usb", 3) == 0) {
        strcpy(emoji, "🔌");  // Use '🔌' for USB devices
    } else {
        strcpy(emoji, "🔧");  // Use '🔧' for other devices
    }
    return emoji;  // Return the determined emoji
}
