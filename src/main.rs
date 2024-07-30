mod file_entry;
mod emoji_utils;
mod display_utils;

use std::env;
use std::fs;
use terminal_size::{Width, Height, terminal_size};
use file_entry::FileEntry;
use display_utils::display_entries;

// Main function: entry point of the program
fn main() {
    // Get and display the current directory
    let current_dir = env::current_dir().unwrap();
    println!("\x1B[1m{}\x1B[0m", current_dir.display());

    // Read directory contents and create FileEntry objects
    let mut entries: Vec<FileEntry> = fs::read_dir(&current_dir)
        .unwrap()
        .filter_map(Result::ok)
        .map(|entry| FileEntry::new(entry.path()))
        .collect();

    // Sort entries
    entries.sort();

    // Get terminal width and display entries
    let (Width(term_width), _) = terminal_size().unwrap_or((Width(80), Height(24)));
    display_entries(&entries, term_width as usize);
}
