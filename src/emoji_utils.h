// src/emoji_utils.h

#ifndef EMOJI_UTILS_H
#define EMOJI_UTILS_H

/**
 * Returns the appropriate emoji for a given file path based on its type.
 *
 * @param path The file path to analyze.
 * @return A string containing the emoji corresponding to the file type.
 *         The caller is responsible for freeing the returned string.
 */
char *get_emoji(const char *path);

/**
 * Determines if the given file path points to an executable file.
 *
 * @param path The file path to check.
 * @return 1 if the file is executable, 0 otherwise.
 */
int is_executable(const char *path);

/**
 * Determines if the given file path points to a text file.
 *
 * @param path The file path to check.
 * @return 1 if the file is a text file, 0 otherwise.
 */
int is_text_file(const char *path);

/**
 * Returns the appropriate emoji for a device file in the /dev directory.
 *
 * @param path The file name in the /dev directory to analyze.
 * @return A string containing the emoji corresponding to the device type.
 *         The caller is responsible for freeing the returned string.
 */
char *get_dev_emoji(const char *path);

#endif // EMOJI_UTILS_H
