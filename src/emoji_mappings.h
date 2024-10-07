/**
 * @file emoji_extensions.h
 * @brief Definitions and declarations for emoji mappings.
 *
 * This header file defines the structure for emoji mappings and declares
 * arrays of mappings for various file types, extensions, and special cases.
 * These mappings are used to assign appropriate emojis to files and directories
 * based on their characteristics.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef EMOJI_EXTENSIONS_H
#define EMOJI_EXTENSIONS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct EmojiMapEntry
 * @brief Structure to map keys (file names or extensions) to emojis.
 */
typedef struct {
    const char *key;   /**< The key (file name, extension, or content pattern) */
    const char *emoji; /**< The corresponding emoji */
} EmojiMapEntry;

/**
 * @brief Array of emoji mappings for file extensions.
 *
 * This array contains mappings of file extensions to their corresponding emojis.
 */
extern const EmojiMapEntry emoji_extension_map[];

/**
 * @brief Size of the emoji_extension_map array.
 */
extern const size_t emoji_extension_map_size;

/**
 * @brief Array of emoji mappings for exact file names.
 *
 * This array contains mappings of specific file names to their corresponding emojis.
 */
extern const EmojiMapEntry emoji_exact_file_map[];

/**
 * @brief Size of the emoji_exact_file_map array.
 */
extern const size_t emoji_exact_file_map_size;

/**
 * @brief Array of emoji mappings for file content patterns.
 *
 * This array contains mappings of file content patterns (e.g., shebangs) to their corresponding emojis.
 */
extern const EmojiMapEntry emoji_file_content_map[];

/**
 * @brief Size of the emoji_file_content_map array.
 */
extern const size_t emoji_file_content_map_size;

/**
 * @brief Array of emoji mappings for exact device file names in /dev.
 *
 * This array contains mappings of specific device file names to their corresponding emojis.
 */
extern const EmojiMapEntry emoji_exact_dev_file_map[];

/**
 * @brief Size of the emoji_exact_dev_file_map array.
 */
extern const size_t emoji_exact_dev_file_map_size;

/**
 * @brief Array of emoji mappings for device file name prefixes in /dev.
 *
 * This array contains mappings of device file name prefixes to their corresponding emojis.
 */
extern const EmojiMapEntry emoji_prefix_dev_map[];

/**
 * @brief Size of the emoji_prefix_dev_map array.
 */
extern const size_t emoji_prefix_dev_map_size;

#ifdef __cplusplus
}
#endif

#endif /* EMOJI_EXTENSIONS_H */
