/**
 * @file emoji_utils.h
 * @brief Utility functions for assigning emojis to files and directories.
 *
 * This header file declares functions for determining appropriate emojis
 * based on file types, attributes, and locations. It supports both regular
 * files and special files in the /dev directory.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef EMOJI_UTILS_H
#define EMOJI_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Determines the appropriate emoji for a given file path.
 *
 * This function analyzes the file at the given path and returns an emoji
 * representing its type (e.g., directory, text file, image, etc.).
 *
 * @param path The file path to analyze.
 * @return A dynamically allocated string containing the emoji.
 *         The caller is responsible for freeing this memory.
 */
char *get_emoji(const char *path);

/**
 * @brief Checks if a file is executable.
 *
 * This function determines whether the file at the given path has
 * execute permissions.
 *
 * @param path The file path to check.
 * @return 1 if the file is executable, 0 otherwise.
 */
int is_executable(const char *path);

/**
 * @brief Checks if a file is a text file.
 *
 * This function analyzes the content of the file to determine if it's
 * likely to be a text file.
 *
 * @param path The file path to check.
 * @return 1 if the file is likely a text file, 0 otherwise.
 */
int is_text_file(const char *path);

/**
 * @brief Determines the appropriate emoji for a device file in the /dev directory.
 *
 * This function analyzes the name of a file in the /dev directory and returns
 * an emoji representing its device type.
 *
 * @param path The file name in the /dev directory to analyze.
 * @return A dynamically allocated string containing the emoji.
 *         The caller is responsible for freeing this memory.
 */
char *get_dev_emoji(const char *path);

#ifdef __cplusplus
}
#endif

#endif /* EMOJI_UTILS_H */
