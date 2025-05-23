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
    {"xip", "📦"},
    {"deb", "📥"},
    {"rpm", "📥"},
    {"py", "🐍"},
    {"js", "🟡"},
    {"css", "🎨"},
    {"cpp", "🔬"},
    {"cc", "🔬"},
    {"cxx", "🔬"},
    {"cppm", "🔬"},
    {"c", "🔬"},
    {"java", "☕"},
    {"jar", "☕"},
    {"pak", "📦"},
    {"go", "🐹"},
    {"rb", "♦️"},
    {"rs", "🦀"},
    {"php", "🐘"},
    {"h", "🧢"},
    {"hpp", "🧢"},
    {"hxx", "🧢"},
    {"xx", "🧢"},
    {"class", "☕"},
    {"kt", "💻"},
    {"scala", "♨️ "},
    {"jsx", "💻"},
    {"tf", "🏗️ "},
    {"tsx", "💻"},
    {"vue", "🟩"},
    {"dart", "🦋"},
    {"lua", "🌙"},
    {"pl", "🐪"},
    {"r", "📈"},
    {"m", "💻"},
    {"mm", "💻"},
    {"asm", "📟"},
    {"s", "📟"},
    {"f", "🅵"},
    {"f90", "🅵"},
    {"lisp", "💻"},
    {"hs", "λ"},
    {"ml", "🐫"},
    {"mli", "🐫"},
    {"clj", "🍃"},
    {"groovy", "💻"},
    {"jl", "💻"},
    {"ex", "⚗️ "},
    {"exs", "⚗️ "},
    {"elm", "💻"},
    {"coffee", "☕"},
    {"ts", "🔷"},
    {"d", "🅳 "},
    {"cs", "💻"},
    {"vb", "💻"},
    {"fs", "🔷"},
    {"fsx", "🔷"},
    {"sql", "🗄️ "},
    {"pas", "🏫"},
    {"lhs", "💻"},
    {"cob", "💻"},
    {"o", "🧩"},
    {"db", "🗄️ "},
    {"gem", "💎"},
    {"txt", "📝"},
    {"rst", "📝"},
    {"log", "🪵"},
    {"ttf", "🔤"},
    {"otf", "🔤"},
    {"woff", "🔤"},
    {"woff2", "🔤"},
    {"pdf", "📕"},
    {"djvu", "📗"},
    {"epub", "📙"},
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
    {"conf", "⚙️ "},
    {"config", "⚙️ "},
    {"toml", "Ⓣ "},
    {"cfg", "⚙️ "},
    {"yaml", "🅈 "},
    {"yml", "🅈 "},
    {"json", "🏝️ "},
    {"jsonc", "🏝️ "},
    {"ini", "🎛️ "},
    {"target", "🎯"},
    {"service", "🚀"},
    {"socket", "🔌"},
    {"vim", "🖖"},
    {"blend", "🧈"},
    {"app", "📱"},
    {"dmg", "💿"},
    {"pkg", "📦"},
    {"patch", "🩹"},
    {"plist", "⚙️ "},
    {"scpt", "📜"},
    {"swift", "🐦"},
    {"xcodeproj", "🛠️"},
    {"mlmodel", "🧠"},
    {"arobject", "🎭"},
    {"cmake", "🏗️ "},
    {"mvn", "🏹"},
    {"html", "🌐"},
    {"xml", "🌐"},
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
    {"zsh-theme", "🆉 "},
    {"gradle", "🐘"},
    {"lock", "🔒"},
    {"mermaid", "🌊"},
    {"plantuml", "🌱"},
    {"dot", "📍"},
    {"drawio", "📉"},
    {"ipa", "📲"},
    {"mlmodel", "🤖"},
    {"icns", "🖼️ "},
    {"tif", "🖼️ "},
    {"framework", "🏗️ "},
    {"xcassets", "🗂️"},
    {"playground", "🎠"},
    {"part", "🧩"},
    {"diff", "🆚"},
    {"bak", "🔙"},
    {"cache", "⏱️"},
    {"desktop", "🖥️ "},
    {"cron", "📅"},
    {"env", "🌍"},
    {"bin", "💾"},
    {"so", "💾"},
    {"pid", "🪪"},
    {"swap", "🔄"},
    {"list", "📋"},
    {"nim", "👑"},
    {"rkt", "🏸"},
    {"deny", "⛔"},
    {"m4", "📻"},
    {"bazel", "🏗️ "},
    {"bzl", "🏗️ "},
    {"zig", "⚡️"},
    {"mk", "🔨"},
    {"xcf", "🖌️ "},
    {"pcm", "💾"},
    {"in", "🚪"},
    {"dsp", "🪟"},
    {"vcxproj", "🪟"},
    {"ico", "🪟"},
    {"ctags", "🏷️"},
    {"awk", "✂️"},
    {"sed", "✂️"},
    {"zst", "🗜️ "},
    {"initramfs", "🛫"},
    {"proto", "📡"},
    {"data", "🔢"},
    {"vala", "🦋"},
    {"vapi", "🦋"},
    {"ui", "🖌️ "},
    {"idx", "📇"},
    {"rev", "🔄"},
    {"pack", "🗜️"},
    {"mo", "🗣️ "},
    {"pf2", "🔠"},
    {"lst", "📜"},
    {"mod", "🧩"},
    {"cocci", "🐞"},
    {"Dockerfile", "🐳"},
    {"csproj", "🔨"},
    {"vcxproj", "🔨"},
    {"vbproj", "🔨"},
    {"fsproj", "🔨"},
    {"pbxproj", "🔨"},
    {"pro", "🔨"},
    {"cbp", "🔨"},
    {"cproject", "🔨"},
    {"dproj", "🔨"},
    {"cbproj", "🔨"},
    {"sum", "🧮"},
    {"nix", "❄️ "},
    {"mjs", "🟡"},
    {"nuspec", "⚙️ "},
    {"iml", "🧩"},
    {"erl", "🟥"},
    {"dump", "🛢️"},
    {"beam", "🔦"},
    {"vhdl", "🧠"},
    {"xdc", "⛓"},
    {"gtkw", "👁️"},
    {"v", "⚡"},
    {"vhd", "📜"},
    {"hex", "📟"},
    {"hash", "🖊️ "},
    {"exr", "🏋️"},
    {"hdr", "🌌"},
    {"psd", "🖌️"},
    {"fossil", "🦎"},
    {"svn", "🆂"},
    {"hg", "☿"}
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
    {"shadow", "🕶️"},
    {"shadow-", "🔙"},
    {"fstab", "⬜"},
    {"Makefile.am", "🏗️ "},
    {"configure.ac", "🏗️ "},
    {"CmakeLists.txt", "🏗️ "},
    {"meson.build", "🏗️ "},
    {"meson_options.txt", "🏗️ "},
    {"WORKSPACE", "🏗️ "},
    {".bazelrc", "🏗️ "},
    {"Makefile.in", "🏗️ "},
    {"configure.ac", "🏗️ "},
    // {".gitignore", "🙈"},
    // {".dockerignore", "🙈"},
    // {".hgignore", "🙈"},
    // {".npmignore", "🙈"},
    // {".bzrignore", "🙈"},
    // {".eslintignore", "🙈"},
    // {".terraformignore", "🙈"},
    // {".prettierignore", "🙈"},
    // {".clang-format-ignore", "🙈"},
    // {".p4ignore", "🙈"},
    // {".bazelignore", "🙈"},
    // {".stylelintignore", "🙈"},
    // {".trackerignore", "🙈"},
    // {".yapfignore", "🙈"},
    {"Dockerfile", "🐳"},
    {"Dockerfile.txt", "🐳"},
    {".gitlab-ci.yml", "🦊"},
    {".travis.yml", "⛑️"},
    {"swagger.yaml", "🧣"},
    {"Jenkinsfile", "🔴"},
    {"tags", "🏷️ "},
    {".hgtags", "🏷️ "},
    //{"LICENSE", "⚖️ "},
    //{"LICENSE.TXT", "⚖️ "},
    //{"LICENSE.txt", "⚖️ "},
    {"COPYING", "⚖️ "},
    {"COPYING.LIB", "⚖️ "},
    //{"LICENSE-docs", "⚖️ "},
    {"APACHE2", "⚖️ "},
    //{"LICENSE-APACHE2", "⚖️ "},
    {".ninja_deps", "🥷"},
    {".ninja_log", "🥷"},
    {".gitattributes", "🐙"},
    {".gitconfig", "🐙"},
    {".gitmodules", "🐙"},
    {".git-blame-ignore-revs", "🐙"},
    {".gitkeep", "🐙"},
    {".gitallowed", "🐙"},
    {".zcompdump", "🆉 "},
    {".zprofile", "🆉 "},
    {".zshrc", "🆉 "},
    {".histfile", "🆉 "},
    {".bashrc", "💰"},
    {".bash_history", "💰"},
    {".bash_logout", "💰"},
    {".bash_profile", "💰"},
    {".prettierrc", "🖌️ "},
    {".inputrc", "🖮 "},
    {".vimrc", "🖖"},
    {".viminfo", "🖖"},
    {".clang-tidy", "🧹"},
    {".clang-format", "🧹"},
    {".clangd", "🧹"},
    {".babelrc", "🗼"},
    {"manifest.txt", "📜"},
    {"robots.txt", "🕷️"},
    {"sitemap.txt", "🗺️"},
    {"Pipfile.txt", "🐍"},
    {"requirements.txt", "🧾"},
    {".editorconfig", "📐"},
    {"tsconfig.json", "🔷"},
    {"tsconfig.test.json", "🧪"},
    {"jest.config.js", "🧪"},
    {"php.ini", "🐘"},
    {"dnsmasq.conf", "🌐"},
    {"vimrc", "🖖"},
    {"virc", "🖖"},
    {"profile", "🚪"},
    {"libuser.conf", "👤"},
    {"subuid", "👤"},
    {"subuid-", "🔙"},
    {"pinforc", "📌"},
    {"sudoers", "👑"},
    {"sudo.conf", "👑"},
    {"man_db.conf", "📖"},
    {"chrony.conf", "⏰"},
    {"group", "👥"},
    {"group-", "🔙"},
    {"zshenv", "🆉 "},
    {"zlogin", "🆉 "},
    {"zlogout", "🆉 "},
    {"zshrc", "🆉 "},
    {"zprofile", "🆉 "},
    {"inittab", "🚀"},
    {"netconfig", "🌐"},
    {"hosts", "🏠"},
    {"login.defs", "🪵"},
    {"logrotate.conf", "🪵"},
    {"rsyslog.conf", "🪵"},
    {"aliases", "🔀"},
    {"dnsmasq.conf", "🎭"},
    {"kdump.conf", "🚛"},
    {"makedumpfile.conf.sample", "🚛"},
    {"subgid", "👥"},
    {"subgid-", "🔙"},
    {"ts.conf", "👆"},
    {"krb5.conf", "🎟️"},
    {"xattr.conf", "🏷️"},
    {"mke2fs.conf", "🌱"},
    {"sysctl.conf", "🎛️ "},
    {"opensc.conf", "💳"},
    {"whois.conf", "🕵️"},
    {"sddm.conf", "🖥️ "},
    {"asound.conf", "🔊"},
    {"passim.conf", "📡"},
    {"vdpau_wrapper.cfg", "🎞️"},
    {"Trolltech.conf", "🔨"},
    {"npmrc", "📦"},
    {"nilfs_cleanerd.conf", "🧹"},
    {"sensors3.conf", "🌡️"},
    {"brltty.conf", "👓"},
    {"magic", "🎩"},
    {"swtpm-localca.options", "🔐"},
    {"filesystems", "🌳"},
    {"passwd", "🛡️"},
    {"passwd-", "🔙"},
    {"bashrc", "💰"},
    {"crontab", "📅"},
    {"Gemfile", "💎"},
    {"Cargo.toml", "🛻"},
    {".eslintrc", "🧹"},
    {"tox.ini", "🧪"},
    {"rsyncd.conf", "🔁"},
    {".cocciconfig", "🐞"},
    {".gdbinit", "🐠"},
    {".flake8", "🥣"},
    {".mailmap", "📧"},
    {"gradle.properties", "🐘"},
    {"build.gradle.kts", "🐘"},
    {".flowconfig", "🧹"},
    {"Doxyfile", "🕮 "},
    {"AUTHORS", "📝"},
    {"index", "📌"},
    {"perf.data", "⏱️"},
    {"perf.data.old", "⏱️"},
    //{"", ""},
};
/** @brief Size of the emoji_exact_file_map array. */
const size_t emoji_exact_file_map_size = sizeof(emoji_exact_file_map) /
                                         sizeof(emoji_exact_file_map[0]);

/**
 * @brief Emoji mappings for not-exact file names.
 *
 * This array maps substring in file names to corresponding emojis. It is used to
 * assign emojis based on not-exact matches of file names, typically for special files.
 */
const EmojiMapEntry emoji_not_exact_file_map[] = {
    {"vmlinuz", "🐧"},
    {"initrd", "🌀"},
    {"System.map", "🗺️ "},
    {"bazel", "🏗️ "},
    {"pylint", "🧪"},
    {"grub", "🥾"},
    {"Kbuild", "🏗️ "},
    {"Kconfig", "🧭"},
    {"Makefile", "🔨"},
    {"ignore", "🙈"},
    {"LICENSE", "⚖️ "},
};

/** @brief Size of the emoji_not_exact_file_map array. */
const size_t emoji_not_exact_file_map_size = sizeof(emoji_not_exact_file_map) /
                                         sizeof(emoji_not_exact_file_map[0]);

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
    {"#!/bin/zsh", "🆉 "},
    {"#!/usr/bin/zsh", "🆉 "},
    {"#!/usr/bin/env zsh", "🆉 "},
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
    {"null", "🕳️ "},
    {"zero", "🕳️ "},
    {"random", "🎲"},
    {"urandom", "🎲"},
    {"usb", "🔌"},
    {"vga_arbiter", "🖼️ "},
    {"vhci", "🔌"},
    {"vhost-net", "🌐"},
    {"vhost-vsock", "💬"},
    {"mcelog", "📋"},
    {"media0", "🎬"},
    {"mei0", "🧠"},
    {"mem", "🗄️ "},
    {"hpet", "⏱️"},
    {"hwrng", "🎲"},
    {"kmsg", "📜"},
    {"kvm", "🌰"},
    {"zram", "🗜️"},
    {"udmabuf", "🔄"},
    {"uhid", "🕹️"},
    {"rfkill", "📡"},
    {"ppp", "🌐"},
    {"ptmx", "🖥️ "},
    {"userfaultfd", "🚧"},
    {"nvram", "🗄️ "},
    {"port", "🔌"},
    {"autofs", "🚗"},
    {"btrfs-control", "🌳"},
    {"console", "🖥️ "},
    {"full", "🔒"},
    {"fuse", "🔥"},
    {"gpiochip0", "📌"},
    {"cuse", "🧩"},
    {"cpu_dma_latency", "⏱️"},
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
    {"tty", "🖥️ "},
    {"usb", "🔌"},
    {"video", "🎥"},
    {"nvme", "💽"},
    {"lp", "🖨️"},
    {"hidraw", "⌨️ "},
    {"vcs", "📟"},
    {"vcsa", "📟"},
    {"ptp", "🕰️ "},
    {"rtc", "🕰️ "},
    {"watchdog", "🐕"},
    {"mtd", "⚡"},
    //{"ttyACM", "♾️ "},
    {"i2c", "🚌"},
    {"dm-", "🧳"},
    {"vhost", "🛗"},

};

/** @brief Size of the emoji_prefix_dev_map array. */
const size_t emoji_prefix_dev_map_size = sizeof(emoji_prefix_dev_map) /
                                         sizeof(emoji_prefix_dev_map[0]);
