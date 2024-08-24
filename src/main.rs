mod file_entry;    // Import file_entry module for FileEntry struct
mod emoji_utils;   // Import emoji_utils module for emoji-related functions
mod display_utils; // Import display_utils module for display functions

use std::env;      // For current_dir() function
use std::fs;       // For read_dir() function
use terminal_size::{Width, Height, terminal_size}; // For getting terminal dimensions
use file_entry::FileEntry; // FileEntry struct for representing file/directory entries
use display_utils::display_entries; // Function to display file entries

// Main function: entry point of the program
fn main() {
    // Get current directory path, unwrap() assumes success (consider error handling in production)
    let current_dir = env::current_dir().unwrap();

    // Print current directory path in bold (ANSI escape codes for formatting)
    println!("\x1B[1m{}\x1B[0m", current_dir.display());

    // Read directory contents, create FileEntry objects, and collect into a vector
    let mut entries: Vec<FileEntry> = fs::read_dir(&current_dir)
        .unwrap() // Assumes read_dir succeeds (consider error handling in production)
        .filter_map(Result::ok) // Keep only successful entries, discard errors
        .map(|entry| FileEntry::new(entry.path())) // Create FileEntry for each path
        .collect(); // Collect results into a vector

    // Sort entries based on FileEntry's implementation of Ord trait
    entries.sort();

    // Get terminal width, default to 80 if unable to determine
    let (Width(term_width), _) = terminal_size().unwrap_or((Width(80), Height(24)));

    // Display entries using the calculated terminal width
    display_entries(&entries, term_width as usize);
}
