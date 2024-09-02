// src/emoji_utils.c

#include <stdio.h>    // printf, fprintf
#include <stdlib.h>   // malloc, free
#include <string.h>   // strcpy, strcmp, strrchr
#include <sys/stat.h> // struct stat, lstat, S_ISLNK, S_ISDIR

#include "emoji_utils.h" // get_emoji, is_executable, is_text_file

#define MAX_PATH 4096 // Maximum path length

typedef struct
{
    const char *key;
    const char *emoji;
} EmojiMapEntry;

/**
 * Safely duplicates a string, logging an error on failure.
 */
static char *safe_strdup(const char *str)
{
    char *dup = strdup(str);
    if (!dup)
    {
        perror("strdup");
    }
    return dup;
}

/**
 * Determines the appropriate emoji for a device file in the /dev directory.
 *
 * @param path The file path in the /dev directory.
 * @return A string containing the corresponding emoji. The caller is responsible for freeing the returned string.
 */
char *get_dev_emoji(const char *path)
{
    static const EmojiMapEntry exact_emoji_map[] = {
        {"loop", "🔁"},
        {"null", "⓿"},
        {"random", "🎲"},
        {"sd", "💽"},
        {"tty", "🖥️"},
        {"urandom", "🎲"},
        {"usb", "🔌"}};

    for (size_t i = 0; i < sizeof(exact_emoji_map) / sizeof(exact_emoji_map[0]); i++)
    {
        if (strcmp(path, exact_emoji_map[i].key) == 0)
        {
            return safe_strdup(exact_emoji_map[i].emoji);
        }
    }

    static const EmojiMapEntry prefix_emoji_map[] = {
        {"loop", "🔁"},
        {"sd", "💽"},
        {"tty", "🖥️"},
        {"usb", "🔌"}};

    for (size_t i = 0; i < sizeof(prefix_emoji_map) / sizeof(prefix_emoji_map[0]); i++)
    {
        if (strncmp(path, prefix_emoji_map[i].key, strlen(prefix_emoji_map[i].key)) == 0)
        {
            return safe_strdup(prefix_emoji_map[i].emoji);
        }
    }

    return safe_strdup("🔧");
}

/**
 * Determines the appropriate emoji for the given file path.
 *
 * @param path The file path to analyze.
 * @return A string containing the corresponding emoji. The caller is responsible for freeing the returned string.
 */
char *get_emoji(const char *path)
{
    struct stat path_stat;
    if (lstat(path, &path_stat) != 0)
    {
        return safe_strdup("❓ ");
    }

    if (S_ISLNK(path_stat.st_mode))
    {
        return safe_strdup(S_ISDIR(path_stat.st_mode) ? "🔗📁 " : "🔗 ");
    }

    if (S_ISDIR(path_stat.st_mode))
    {
        return safe_strdup("📁 ");
    }

    char *extension = strrchr(path, '.');
    if (extension)
    {
        extension++; // Skip the dot
        static const EmojiMapEntry ext_map[] = {
            {"7z", "📦 "},
            {"aac", "🎧 "},
            {"aiff", "🎧 "},
            {"apk", "📱 "},
            {"avi", "🎬 "},
            {"bat", "⚙️  "},
            {"bin", "💾 "},
            {"bmp", "📸 "},
            {"bz2", "📦 "},
            {"c", "💻 "},
            {"cbr", "📚 "},
            {"cbz", "📚 "},
            {"cer", "🔑 "},
            {"cmd", "⚙️  "},
            {"conf", "⚙️  "},
            {"config", "⚙️  "},
            {"cpp", "💻 "},
            {"crt", "🔑 "},
            {"csv", "📊 "},
            {"deb", "📦 "},
            {"dmg", "💽 "},
            {"djvu", "📚 "},
            {"doc", "📄 "},
            {"docx", "📄 "},
            {"epub", "📚 "},
            {"exe", "🚀 "},
            {"flac", "🎧 "},
            {"gif", "📸 "},
            {"go", "💻 "},
            {"gz", "📦 "},
            {"h", "💻 "},
            {"hpp", "💻 "},
            {"html", "💻 "},
            {"ico", "🖼️ "},
            {"img", "💽 "},
            {"ini", "⚙️  "},
            {"iso", "💽 "},
            {"jar", "🔧 "},
            {"java", "💻 "},
            {"jpeg", "📸 "},
            {"jpg", "📸 "},
            {"js", "💻 "},
            {"json", "⚙️  "},
            {"log", "📝 "},
            {"lzma", "📦 "},
            {"m2ts", "🎬 "},
            {"m4a", "🎧 "},
            {"md", "📝 "},
            {"mkv", "🎬 "},
            {"mov", "🎬 "},
            {"mp3", "🎧 "},
            {"mp4", "🎬 "},
            {"msi", "💽 "},
            {"o", "🧩 "},
            {"odt", "📄 "},
            {"otf", "🔤 "},
            {"pdf", "📚 "},
            {"pem", "🔑 "},
            {"php", "💻 "},
            {"pkg", "📦 "},
            {"png", "📸 "},
            {"ppt", "📄 "},
            {"pptx", "📄"},
            {"psd", "🖌️ "},
            {"pub", "🔑 "},
            {"py", "💻 "},
            {"qcow", "🐮 "},
            {"qcow2", "🐮 "},
            {"rar", "📦 "},
            {"rst", "📝 "},
            {"rtf", "📄 "},
            {"sh", "⚙️  "},
            {"sql", "🗃️ "},
            {"svg", "📸 "},
            {"swift", "💻 "},
            {"tar", "📦 "},
            {"tiff", "🖼️ "},
            {"toml", "⚙️  "},
            {"ts", "💻 "},
            {"ttf", "🔤 "},
            {"txt", "📝 "},
            {"vb", "💻 "},
            {"vbs", "💻 "},
            {"wav", "🎧 "},
            {"webm", "🎬 "},
            {"webp", "📸 "},
            {"woff", "🔤 "},
            {"woff2", "🔤 "},
            {"xls", "📄 "},
            {"xlsx", "📄 "},
            {"xml", "🗃️ "},
            {"yaml", "⚙️ "},
            {"yml", "⚙️  "},
            {"zip", "📦 "},
            {"zsh", "⚙️  "}};

        for (size_t i = 0; i < sizeof(ext_map) / sizeof(ext_map[0]); i++)
        {
            if (strcmp(extension, ext_map[i].key) == 0)
            {
                return safe_strdup(ext_map[i].emoji);
            }
        }
    }

    if (path[0] == '.')
    {
        return safe_strdup("⚙️  ");
    }

    if (is_executable(path))
    {
        return safe_strdup("💾 ");
    }

    if (is_text_file(path))
    {
        return safe_strdup("📝 ");
    }

    return safe_strdup("❓ ");
}

/**
 * Determines if the given file path points to an executable file.
 */
int is_executable(const char *path)
{
    struct stat st;
    if (stat(path, &st) == 0)
    {
        return (st.st_mode & S_IXUSR) != 0;
    }
    return 0;
}

/**
 * Determines if the given file path points to a text file.
 */
int is_text_file(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        return 0;
    }

    unsigned char buffer[512];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    for (size_t i = 0; i < bytesRead; i++)
    {
        if (buffer[i] < 0x09 || (buffer[i] > 0x0D && buffer[i] < 0x20))
        {
            return 0;
        }
    }
    return 1;
}
