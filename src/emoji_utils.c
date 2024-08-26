#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include "emoji_utils.h"

#define MAX_PATH 4096

char* get_emoji(const char* path) {
    struct stat path_stat;
    char* emoji = malloc(5 * sizeof(char));
    if (emoji == NULL) {
        return NULL;
    }

    if (lstat(path, &path_stat) != 0) {
        strcpy(emoji, "❓");
        return emoji;
    }

    if (S_ISLNK(path_stat.st_mode)) {
        if (S_ISDIR(path_stat.st_mode)) {
            strcpy(emoji, "🔗📁");
        } else {
            strcpy(emoji, "🔗");
        }
        return emoji;
    }

    if (S_ISDIR(path_stat.st_mode)) {
        strcpy(emoji, "📁");
        return emoji;
    }

    char* extension = strrchr(path, '.');
    if (extension != NULL) {
        extension++;
    }

    if (extension != NULL) {
        if (strcmp(extension, "md") == 0) strcpy(emoji, "📑");
        else if (strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0 ||
                 strcmp(extension, "png") == 0 || strcmp(extension, "gif") == 0 ||
                 strcmp(extension, "bmp") == 0 || strcmp(extension, "svg") == 0 ||
                 strcmp(extension, "webp") == 0) strcpy(emoji, "📸");
        else if (strcmp(extension, "mp4") == 0 || strcmp(extension, "avi") == 0 ||
                 strcmp(extension, "mkv") == 0 || strcmp(extension, "mov") == 0 ||
                 strcmp(extension, "flv") == 0 || strcmp(extension, "wmv") == 0 ||
                 strcmp(extension, "webm") == 0) strcpy(emoji, "🎬");
        else if (strcmp(extension, "mp3") == 0 || strcmp(extension, "wav") == 0 ||
                 strcmp(extension, "ogg") == 0 || strcmp(extension, "flac") == 0 ||
                 strcmp(extension, "m4a") == 0 || strcmp(extension, "aac") == 0) strcpy(emoji, "🎧");
        else if (strcmp(extension, "zip") == 0 || strcmp(extension, "tar") == 0 ||
                 strcmp(extension, "gz") == 0 || strcmp(extension, "bz2") == 0 ||
                 strcmp(extension, "xz") == 0 || strcmp(extension, "7z") == 0 ||
                 strcmp(extension, "rar") == 0) strcpy(emoji, "📦");
        else if (strcmp(extension, "deb") == 0 || strcmp(extension, "rpm") == 0) strcpy(emoji, "📥");
        else if (strcmp(extension, "py") == 0 || strcmp(extension, "sh") == 0 ||
                 strcmp(extension, "js") == 0 || strcmp(extension, "html") == 0 ||
                 strcmp(extension, "css") == 0 || strcmp(extension, "cpp") == 0 ||
                 strcmp(extension, "c") == 0 || strcmp(extension, "java") == 0 ||
                 strcmp(extension, "go") == 0 || strcmp(extension, "rb") == 0 ||
                 strcmp(extension, "rs") == 0 || strcmp(extension, "php") == 0 ||
                 strcmp(extension, "h") == 0 || strcmp(extension, "hpp") == 0) strcpy(emoji, "💻");
        else if (strcmp(extension, "o") == 0) strcpy(emoji, "🧩");
        else if (strcmp(extension, "txt") == 0 || strcmp(extension, "rst") == 0 ||
                 strcmp(extension, "log") == 0) strcpy(emoji, "📝");
        else if (strcmp(extension, "ttf") == 0 || strcmp(extension, "otf") == 0 ||
                 strcmp(extension, "woff") == 0 || strcmp(extension, "woff2") == 0) strcpy(emoji, "🔤");
        else if (strcmp(extension, "pdf") == 0 || strcmp(extension, "djvu") == 0 ||
                 strcmp(extension, "epub") == 0) strcpy(emoji, "📚");
        else if (strcmp(extension, "pem") == 0 || strcmp(extension, "crt") == 0 ||
                 strcmp(extension, "key") == 0 || strcmp(extension, "pub") == 0 ||
                 strcmp(extension, "p12") == 0) strcpy(emoji, "🔑");
        else if (strcmp(extension, "csv") == 0) strcpy(emoji, "📊");
        else if (strcmp(extension, "torrent") == 0) strcpy(emoji, "🌊");
        else if (strcmp(extension, "iso") == 0 || strcmp(extension, "img") == 0) strcpy(emoji, "💽");
        else if (strcmp(extension, "qcow") == 0 || strcmp(extension, "qcow2") == 0) strcpy(emoji, "🐮");
        else if (strcmp(extension, "vv") == 0) strcpy(emoji, "🕹️");
        else if (strcmp(extension, "doc") == 0 || strcmp(extension, "docx") == 0 ||
                 strcmp(extension, "odt") == 0 || strcmp(extension, "rtf") == 0 ||
                 strcmp(extension, "xls") == 0 || strcmp(extension, "xlsx") == 0 ||
                 strcmp(extension, "ods") == 0 || strcmp(extension, "ppt") == 0 ||
                 strcmp(extension, "pptx") == 0 || strcmp(extension, "odp") == 0) strcpy(emoji, "📄");
        else if (strcmp(extension, "conf") == 0 || strcmp(extension, "config") == 0 ||
                 strcmp(extension, "toml") == 0 || strcmp(extension, "cfg") == 0 ||
                 strcmp(extension, "yaml") == 0 || strcmp(extension, "yml") == 0 ||
                 strcmp(extension, "json") == 0 || strcmp(extension, "ini") == 0) strcpy(emoji, "⚙️");
        else {
            if (path[0] == '.') {
                strcpy(emoji, "⚙️");
            } else if (is_executable(path)) {
                strcpy(emoji, "💾");
            } else if (is_text_file(path)) {
                strcpy(emoji, "📝");
            } else {
                strcpy(emoji, "❓");
            }
        }
    } else {
        if (path[0] == '.') {
            strcpy(emoji, "⚙️");
        } else if (is_executable(path)) {
            strcpy(emoji, "💾");
        } else if (is_text_file(path)) {
            strcpy(emoji, "📝");
        } else {
            strcpy(emoji, "❓");
        }
    }

    return emoji;
}

int is_executable(const char* path) {
    struct stat st;
    if (stat(path, &st) < 0) {
        return 0;
    }
    return (st.st_mode & S_IXUSR) != 0;
}

int is_text_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        return 0;
    }

    unsigned char buffer[1024];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    if (bytes_read == 0) {
        return 1;
    }

    for (size_t i = 0; i < bytes_read; i++) {
        if (!isprint(buffer[i]) && !isspace(buffer[i])) {
            return 0;
        }
    }

    return 1;
}

char* get_dev_emoji(const char* path) {
    char* filename = strrchr(path, '/');
    if (filename == NULL) {
        filename = (char*)path;
    } else {
        filename++;
    }

    char* emoji = malloc(5 * sizeof(char));
    if (emoji == NULL) {
        return NULL;
    }

    if (strcmp(filename, "null") == 0 || strcmp(filename, "zero") == 0) {
        strcpy(emoji, "⓿");
    } else if (strcmp(filename, "random") == 0 || strcmp(filename, "urandom") == 0) {
        strcpy(emoji, "🎲");
    } else if (strncmp(filename, "tty", 3) == 0) {
        strcpy(emoji, "🖥️");
    } else if (strncmp(filename, "sd", 2) == 0) {
        strcpy(emoji, "💽");
    } else if (strncmp(filename, "loop", 4) == 0) {
        strcpy(emoji, "🔁");
    } else if (strncmp(filename, "usb", 3) == 0) {
        strcpy(emoji, "🔌");
    } else {
        strcpy(emoji, "🔧");
    }

    return emoji;
}
