/**
 * @file emoji_extensions.c
 * @brief Implementation of emoji mappings for various file types and attributes.
 *
 * This file contains the definitions of emoji mapping arrays for file extensions,
 * exact file names, file content patterns, and device files. These mappings are
 * used to assign appropriate emojis to files and directories based on their
 * characteristics.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include "emoji_mappings.h"

/**
 * @brief Emoji mappings for file extensions.
 *
 * This array maps file extensions to corresponding emojis. It is used to
 * assign emojis based on the file type indicated by its extension.
 */
const EmojiMapEntry emoji_extension_map[] = {
    {"md", "📑"},
    {"jpg", "📸"},
    {"jpeg", "📸"},
    {"png", "📸"},
    {"gif", "📸"},
    {"bmp", "📸"},
    {"svg", "📸"},
    {"webp", "📸"},
    {"mp4", "🎬"},
    {"avi", "🎬"},
    {"mkv", "🎬"},
    {"mov", "🎬"},
    {"flv", "🎬"},
    {"wmv", "🎬"},
    {"webm", "🎬"},
    {"mp3", "🎧"},
    {"wav", "🎧"},
    {"ogg", "🎧"},
    {"flac", "🎧"},
    {"m4a", "🎧"},
    {"aac", "🎧"},
    {"zip", "📦"},
    {"tar", "📦"},
    {"gz", "📦"},
    {"bz2", "📦"},
    {"xz", "📦"},
    {"7z", "📦"},
    {"rar", "📦"},
    {"deb", "📥"},
    {"rpm", "📥"},
    {"py", "🐍"},
    {"sh", "💻"},
    {"js", "💻"},
    {"css", "🎨"},
    {"cpp", "🔬"},
    {"c", "🔬"},
    {"java", "☕"},
    {"go", "🐹"},
    {"rb", "♦️"},
    {"rs", "🦀"},
    {"php", "🐘"},
    {"h", "🧢"},
    {"hpp", "🧢"},
    {"class", "☕"},
    {"kt", "💻"},
    {"scala", "📐"},
    {"jsx", "💻"},
    {"tf", "🏗️"},
    {"tsx", "💻"},
    {"vue", "🟩"},
    {"dart", "🦋"},
    {"lua", "💻"},
    {"pl", "🐪"},
    {"r", "📈"},
    {"m", "💻"},
    {"mm", "💻"},
    {"asm", "💻"},
    {"s", "💻"},
    {"f", "🅵"},
    {"f90", "🅵"},
    {"lisp", "💻"},
    {"hs", "💻"},
    {"ml", "Ⓜ️"},
    {"clj", "💻"},
    {"groovy", "💻"},
    {"jl", "💻"},
    {"ex", "💻"},
    {"exs", "💻"},
    {"elm", "💻"},
    {"coffee", "☕"},
    {"ts", "🅃 "},
    {"d", "🅳 "},
    {"cs", "💻"},
    {"vb", "💻"},
    {"fs", "💻"},
    {"sql", "🗄️"},
    {"pas", "🏫"},
    {"lhs", "💻"},
    {"cob", "💻"},
    {"o", "🧩"},
    {"db", "🗄️"},
    {"mod", "🐹"},
    {"gem", "💎"},
    {"txt", "📝"},
    {"rst", "📝"},
    {"log", "🪵"},
    {"ttf", "🔤"},
    {"otf", "🔤"},
    {"woff", "🔤"},
    {"woff2", "🔤"},
    {"pdf", "📚"},
    {"djvu", "📚"},
    {"epub", "📚"},
    {"pem", "🔑"},
    {"crt", "🔑"},
    {"key", "🔑"},
    {"pub", "🔑"},
    {"p12", "🔑"},
    {"csv", "📊"},
    {"torrent", "🌊"},
    {"iso", "💽"},
    {"img", "💽"},
    {"qcow", "🐮"},
    {"qcow2", "🐮"},
    {"vv", "🕹️"},
    {"doc", "📄"},
    {"docx", "📄"},
    {"odt", "📄"},
    {"rtf", "📄"},
    {"xls", "📄"},
    {"xlsx", "📄"},
    {"ods", "📄"},
    {"ppt", "📄"},
    {"pptx", "📄"},
    {"odp", "📄"},
    {"conf", "⚙️"},
    {"config", "⚙️"},
    {"toml", "⚙️"},
    {"cfg", "⚙️"},
    {"yaml", "🅈 "},
    {"yml", "🅈 "},
    {"json", "🏝️"},
    {"ini", "⚙️"},
    {"target", "🎯"},
    {"service", "🚀"},
    {"socket", "🔌"},
    {"vim", "🖖"},
    {"blend", "🧈"},
    {"app", "📱"},
    {"dmg", "💿"},
    {"pkg", "📦"},
    {"patch", "🩹"},
    {"plist", "📋"},
    {"scpt", "📜"},
    {"swift", "🐦"},
    {"xcodeproj", "🛠️"},
    {"mlmodel", "🧠"},
    {"arobject", "🎭"},
    {"cmake", "🏭"},
    {"mvn", "🏹"},
    {"html", "🌐"},
    {"sks", "🎮"},
    {"car", "🚗"},
    {"xcassets", "🗂️"},
    {"dSYM", "🐛"},
    {"terminal", "🖥️"},
    {"lock", "🔒"},
    {"webloc", "🔗"},
    {"workflow", "🔄"},
    {"apk", "📱"},
    {"rc", "👟"},
    {"bundle", "🎁"},
    {"pb", "📋"},
    {"sock", "🧦"},
    {"tmp", "⏳"},
    {"ko", "🌰"},
    {"ccl", "🎨"},
    {"sh", "🐚"},
    {"bash", "💰"},
    {"fish", "🐟"},
    {"xib", "🖼️"},
    {"ninja", "🥷"},
    {"lisp", "λ"},
    {"cl", "λ"},
    {"lsp", "λ"},
    {"ada", "✈️"},
    {"adb", "✈️"},
    {"ads", "✈️"},
    {"zsh", "🆉 "},
    {"gradle", "🐘"},
    {"lock", "🔒"},
    {"mermaid", "🌊"},
    {"plantuml", "🌱"},
    {"dot", "📍"},
    {"drawio", "📉"},
    {"ipa", "📲"},
    {"xcodeproj", "🛠️"},
    {"mlmodel", "🤖"},
    {"icns", "🖼️"},
    {"framework", "🏗️"},
    {"xcassets", "🗂️"},
    {"playground", "🎠"},
    {"part", "🧩"},
    {"diff", "🆚"},
    {"bak", "🔙"},
    {"cache", "⏱️"},
    {"desktop", "🖥️"},
    {"cron", "📅"},
    {"env", "🌍"},
    {"bin", "💾"},
    {"pid", "🪪"},
    {"swap", "🔄"}
};

/** @brief Size of the emoji_extension_map array. */
const size_t emoji_extension_map_size = sizeof(emoji_extension_map) /
                                        sizeof(emoji_extension_map[0]);

/**
 * @brief Emoji mappings for exact file names.
 *
 * This array maps specific file names to corresponding emojis. It is used to
 * assign emojis based on exact matches of file names, typically for special files.
 */
const EmojiMapEntry emoji_exact_file_map[] = {
    {"vmlinuz", "🐧"},
    {"grub", "🥾"},
    {"shadow", "🕶️"},
    {"fstab", "⬜"},
    {"Makefile", "🧰"},
    {"Makefile.am", "🏭"},
    {"configure.ac", "🏭"},
    {"CmakeLists.txt", "🏭"},
    {"meson.build", "🏭"},
    {".gitignore", "🙈"},
    {".dockerignore", "🙈"},
    {".hgignore", "🙈"},
    {".npmignore", "🙈"},
    {".bzrignore", "🙈"},
    {".eslintignore", "🙈"},
    {".terraformignore", "🙈"},
    {".prettierignore", "🙈"},
    {".p4ignore", "🙈"},
    {"Dockerfile", "🐳"},
    {".gitlab-ci.yml", "🦊"},
    {".travis.yml", "⛑️"},
    {"swagger.yaml", "🧣"},
    {"Jenkinsfile", "🔴"},
    {"tags", "🏷️"},
    {"LICENSE", "⚖️"},
    {".ninja_deps", "🥷"},
    {".ninja_log", "🥷"}
};
/** @brief Size of the emoji_exact_file_map array. */
const size_t emoji_exact_file_map_size = sizeof(emoji_exact_file_map) /
                                         sizeof(emoji_exact_file_map[0]);

/**
 * @brief Emoji mappings for file content patterns.
 *
 * This array maps file content patterns (typically shebang lines) to corresponding emojis.
 * It is used to assign emojis based on the content of script files.
 */
const EmojiMapEntry emoji_file_content_map[] = {
    {"#!/bin/sh", "🐚"},
    {"#!/usr/bin/sh", "🐚"},
    {"#!/usr/bin/env sh", "🐚"},
    {"#!/bin/bash", "💰"},
    {"#!/usr/bin/bash", "💰"},
    {"#!/usr/bin/env bash", "💰"},
    {"#!/bin/dash", "🐚"},
    {"#!/usr/bin/dash", "🐚"},
    {"#!/usr/bin/env dash", "🐚"},
    {"#!/bin/zsh", "🆉"},
    {"#!/usr/bin/zsh", "🆉"},
    {"#!/usr/bin/env zsh", "🆉"},
    {"#!/bin/ksh", "🐚"},
    {"#!/usr/bin/ksh", "🐚"},
    {"#!/usr/bin/env ksh", "🐚"},
    {"#!/usr/bin/perl", "🐪"},
    {"#!/usr/bin/perl5", "🐪"},
    {"#!/usr/local/bin/perl", "🐪"},
    {"#!/usr/bin/env perl", "🐪"},
    {"#!/usr/bin/env perl5", "🐪"},
    {"#!/opt/bin/perl", "🐪"},
    {"#!/bin/ruby", "♦️"},
    {"#!/usr/bin/ruby", "♦️"},
    {"#!/usr/local/bin/ruby", "♦️"},
    {"#!/usr/bin/env ruby", "♦️"},
    {"#!/opt/local/bin/ruby", "♦️"},
    {"#!/usr/bin/python", "🐍"},
    {"#!/usr/bin/python2", "🐍"},
    {"#!/usr/bin/python3", "🐍"},
    {"#!/usr/local/bin/python", "🐍"},
    {"#!/usr/bin/env python", "🐍"},
    {"#!/usr/bin/env python2", "🐍"},
    {"#!/usr/bin/env python3", "🐍"},
    {"#!/usr/bin/lua", "🌙"},
    {"#!/usr/local/bin/lua", "🌙"},
    {"#!/usr/bin/env lua", "🌙"},
    {"#!/usr/bin/tcl", "☯️"},
    {"#!/usr/local/bin/tcl", "☯️"},
    {"#!/usr/bin/env tcl", "☯️"},
    {"#!/usr/bin/awk", "🐦"},
    {"#!/usr/bin/awk -f", "🐦"},
    {"#!/usr/local/bin/awk", "🐦"},
    {"#!/usr/bin/env awk", "🐦"},
    {"#!/usr/bin/gawk", "🐦"},
    {"#!/usr/bin/env gawk", "🐦"},
    {"#!/usr/bin/node", "💚"},
    {"#!/usr/local/bin/node", "💚"},
    {"#!/usr/bin/env node", "💚"},
    {"#!/usr/bin/nodejs", "💚"},
    {"#!/usr/bin/env nodejs", "💚"},
    {"#!/usr/bin/php", "🐘"},
    {"#!/usr/local/bin/php", "🐘"},
    {"#!/usr/bin/env php", "🐘"},
    {"#!/usr/bin/fish", "🐟"},
    {"#!/usr/local/bin/fish", "🐟"},
    {"#!/usr/bin/env fish", "🐟"}
};
/** @brief Size of the emoji_file_content_map array. */
const size_t emoji_file_content_map_size = sizeof(emoji_file_content_map) /
                                           sizeof(emoji_file_content_map[0]);


/**
 * @brief Emoji mappings for exact device file names in /dev.
 *
 * This array maps specific device file names in the /dev directory to corresponding emojis.
 * It is used to assign emojis to known device files.
 */
const EmojiMapEntry emoji_exact_dev_file_map[] = {
    {"loop", "🔁"},
    {"null", "🕳️"},
    {"zero", "🕳️"},
    {"random", "🎲"},
    {"urandom", "🎲"},
    {"tty", "🖥️"},
    {"usb", "🔌"},
    {"vga_arbiter", "🖼️"},
    {"vhci", "🔌"},
    {"vhost-net", "🌐"},
    {"vhost-vsock", "💬"},
    {"mcelog", "📋"},
    {"media0", "🎬"},
    {"mei0", "🧠"},
    {"mem", "🗄️"},
    {"hpet", "⏱️"},
    {"hwrng", "🎲"},
    {"kmsg", "📜"},
    {"kvm", "🌰"},
    {"zram", "🗜️"},
    {"udmabuf", "🔄"},
    {"uhid", "🕹️"},
    {"rfkill", "📡"},
    {"ppp", "🌐"},
    {"ptmx", "🖥️"},
    {"userfaultfd", "🚧"},
    {"nvram", "🗄️"},
    {"port", "🔌"},
    {"autofs", "🚗"},
    {"btrfs-control", "🌳"},
    {"console", "🖥️"},
    {"full", "🔒"},
    {"fuse", "🔥"},
    {"gpiochip0", "📌"},
    {"cuse", "🧩"},
    {"cpu_dma_latency", "⏱️"}
};
/** @brief Size of the emoji_exact_dev_file_map array. */
const size_t emoji_exact_dev_file_map_size = sizeof(emoji_exact_dev_file_map) /
                                             sizeof(emoji_exact_dev_file_map[0]);

/**
 * @brief Emoji mappings for device file name prefixes in /dev.
 *
 * This array maps prefixes of device file names in the /dev directory to corresponding emojis.
 * It is used to assign emojis to device files based on their name prefixes.
 */
const EmojiMapEntry emoji_prefix_dev_map[] = {
    {"loop", "🔁"},
    {"sd", "💽"},
    {"tty", "🖥️"},
    {"usb", "🔌"},
    {"video", "🎥"},
    {"nvme", "💽"},
    {"lp", "🖨️"},
    {"hidraw", "🔠"},
    {"vcs", "📟"},
    {"vcsa", "📟"},
    {"ptp", "🕰️"},
    {"rtc", "🕰️"},
    {"watchdog", "🐕"},
    {"mtd", "⚡"}
};

/** @brief Size of the emoji_prefix_dev_map array. */
const size_t emoji_prefix_dev_map_size = sizeof(emoji_prefix_dev_map) /
                                         sizeof(emoji_prefix_dev_map[0]);