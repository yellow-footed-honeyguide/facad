// src/emoji_utils.c
#include <ctype.h>
#include <stdio.h>     // printf, fprintf
#include <stdlib.h>    // malloc, free
#include <string.h>    // strcpy, strcmp, strrchr
#include <sys/stat.h>  // struct stat, lstat, S_ISLNK, S_ISDIR

#include "emoji_utils.h"  // get_emoji, is_executable, is_text_file

#define MAX_PATH 4096

typedef struct {
    const char *key;
    const char *emoji;
} EmojiMapEntry;

// Safe string duplication function
static char *safe_strdup(const char *str) {
    char *dup = strdup(str);
    if (!dup) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    return dup;
}

// Get emoji for /dev files
char *get_dev_emoji(const char *path) {
    static const EmojiMapEntry exact_emoji_map[] = {
        {"loop", "🔁"},      {"null", "🕳️"},        {"zero", "🕳️"},
        {"random", "🎲"},    {"urandom", "🎲"},     {"tty", "🖥️"},
        {"usb", "🔌"},       {"vga_arbiter", "🖼️"}, {"vhci", "🔌"},
        {"vhost-net", "🌐"}, {"vhost-vsock", "💬"}, {"mcelog", "📋"},
        {"media0", "🎬"},    {"mei0", "🧠"},        {"mem", "🗄️"},
        {"hpet", "⏱️"},       {"hwrng", "🎲"},       {"kmsg", "📜"},
        {"kvm", "🌰"},       {"zram", "🗜️"},        {"udmabuf", "🔄"},
        {"uhid", "🕹️"},      {"rfkill", "📡"},      {"ppp", "🌐"},
        {"ptmx", "🖥️"},      {"userfaultfd", "🚧"}, {"nvram", "🗄️"},
        {"port", "🔌"},      {"autofs", "🚗"},      {"btrfs-control", "🌳"},
        {"console", "🖥️"},   {"full", "🔒"},        {"fuse", "🔥"},
        {"gpiochip0", "📌"}, {"cuse", "🧩"},        {"cpu_dma_latency", "⏱️"}
    };

    for (size_t i = 0; i < sizeof(exact_emoji_map) / sizeof(exact_emoji_map[0]); i++) {
        if (strcmp(path, exact_emoji_map[i].key) == 0) {
            return safe_strdup(exact_emoji_map[i].emoji);
        }
    }

    static const EmojiMapEntry prefix_emoji_map[] = {
        {"loop", "🔁"}, {"sd", "💽"},  {"tty", "🖥️"},      {"usb", "🔌"}, {"video", "🎥"},
        {"nvme", "💽"}, {"lp", "🖨️"},  {"hidraw", "🔠"},   {"vcs", "📟"}, {"vcsa", "📟"},
        {"ptp", "🕰️"},  {"rtc", "🕰️"}, {"watchdog", "🐕"}, {"mtd", "⚡"}
    };

    for (size_t i = 0; i < sizeof(prefix_emoji_map) / sizeof(prefix_emoji_map[0]); i++) {
        if (strncmp(path, prefix_emoji_map[i].key, strlen(prefix_emoji_map[i].key)) == 0) {
            return safe_strdup(prefix_emoji_map[i].emoji);
        }
    }

    return safe_strdup("🔧");
}

// Get emoji for a file based on its characteristics
char *get_emoji(const char *path) {
    struct stat path_stat;
    if (lstat(path, &path_stat) != 0) {
        return safe_strdup("❓");
    }

    if (S_ISLNK(path_stat.st_mode)) {
        return safe_strdup(S_ISDIR(path_stat.st_mode) ? "🔗📁" : "🔗");
    }

    if (S_ISDIR(path_stat.st_mode)) {
        return safe_strdup("📁");
    }

    // Check for special cases
    const char *filename = strrchr(path, '/');
    filename = filename ? filename + 1 : path;

    static const EmojiMapEntry special_case_map[] = {
        {"vmlinuz", "🐧"},    // Linux kernel
        {"grub", "🥾"},       // GRUB bootloader
        {"shadow", "🕶️"},     // Shadow password file
        {"fstab", "⬜"}       // Filesystem table
    };

    for (size_t i = 0; i < sizeof(special_case_map) / sizeof(special_case_map[0]); i++) {
        if (strstr(filename, special_case_map[i].key) == filename) {
            return safe_strdup(special_case_map[i].emoji);
        }
    }

    char *extension = strrchr(path, '.');
    if (extension) {
        extension++;  // Skip the dot
        static const EmojiMapEntry ext_map[] = {
            {"md", "📑"},    {"jpg", "📸"},     {"jpeg", "📸"}, {"png", "📸"},   {"gif", "📸"},
            {"bmp", "📸"},   {"svg", "📸"},     {"webp", "📸"}, {"mp4", "🎬"},   {"avi", "🎬"},
            {"mkv", "🎬"},   {"mov", "🎬"},     {"flv", "🎬"},  {"wmv", "🎬"},   {"webm", "🎬"},
            {"mp3", "🎧"},   {"wav", "🎧"},     {"ogg", "🎧"},  {"flac", "🎧"},  {"m4a", "🎧"},
            {"aac", "🎧"},   {"zip", "📦"},     {"tar", "📦"},  {"gz", "📦"},    {"bz2", "📦"},
            {"xz", "📦"},    {"7z", "📦"},      {"rar", "📦"},  {"deb", "📥"},   {"rpm", "📥"},
            {"py", "💻"},    {"sh", "💻"},      {"js", "💻"},   {"html", "💻"},  {"css", "💻"},
            {"cpp", "💻"},   {"c", "💻"},       {"java", "💻"}, {"go", "💻"},    {"rb", "💻"},
            {"rs", "💻"},    {"php", "💻"},     {"h", "💻"},    {"hpp", "💻"},   {"o", "🧩"},
            {"txt", "📝"},   {"rst", "📝"},     {"log", "📝"},  {"ttf", "🔤"},   {"otf", "🔤"},
            {"woff", "🔤"},  {"woff2", "🔤"},   {"pdf", "📚"},  {"djvu", "📚"},  {"epub", "📚"},
            {"pem", "🔑"},   {"crt", "🔑"},     {"key", "🔑"},  {"pub", "🔑"},   {"p12", "🔑"},
            {"csv", "📊"},   {"torrent", "🌊"}, {"iso", "💽"},  {"img", "💽"},   {"qcow", "🐮"},
            {"qcow2", "🐮"}, {"vv", "🕹️"},      {"doc", "📄"},  {"docx", "📄"},  {"odt", "📄"},
            {"rtf", "📄"},   {"xls", "📄"},     {"xlsx", "📄"}, {"ods", "📄"},   {"ppt", "📄"},
            {"pptx", "📄"},  {"odp", "📄"},     {"conf", "⚙️"},  {"config", "⚙️"}, {"toml", "⚙️"},
            {"cfg", "⚙️"},    {"yaml", "⚙️"},     {"yml", "⚙️"},   {"json", "⚙️"},   {"ini", "⚙️"},
						{"target", "🎯"}, {"service", "🚀"}, {"socket", "🔁"}
        };

        for (size_t i = 0; i < sizeof(ext_map) / sizeof(ext_map[0]); i++) {
            if (strcasecmp(extension, ext_map[i].key) == 0) {
                return safe_strdup(ext_map[i].emoji);
            }
        }
    }

    if (path[0] == '.') {
        return safe_strdup("⚙️");
    }

    if (is_executable(path)) {
        return safe_strdup("💾");
    }

    if (is_text_file(path)) {
        return safe_strdup("📝");
    }

    return safe_strdup("❓");
}

// Check if a file is executable
int is_executable(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return (st.st_mode & S_IXUSR) != 0;
    }
    return 0;
}

// Check if a file is a text file
int is_text_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        return 0;
    }

    unsigned char buffer[1024];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    if (bytesRead == 0) {
        return 1;  // Empty file is considered text
    }

    for (size_t i = 0; i < bytesRead; i++) {
        if (!isprint(buffer[i]) && !isspace(buffer[i])) {
            return 0;
        }
    }
    return 1;
}
