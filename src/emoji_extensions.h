#ifndef EMOJI_EXTENSIONS_H
#define EMOJI_EXTENSIONS_H

#include <stddef.h>

// Define a structure to map keys (file names or extensions) to emojis
typedef struct {
    const char *key;
    const char *emoji;
} EmojiMapEntry;

extern const EmojiMapEntry ext_map[];
extern const size_t ext_map_size;

extern const EmojiMapEntry special_case_map[];
extern const size_t special_case_map_size;

extern const EmojiMapEntry content_map[];
extern const size_t content_map_size;

#endif // EMOJI_EXTENSIONS_H
