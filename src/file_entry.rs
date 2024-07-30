use std::path::PathBuf;
use std::cmp::Ordering;
use crate::emoji_utils::{get_emoji, is_executable};

pub struct FileEntry {
    pub emoji: String,
    pub name: String,
    pub path: PathBuf,
    is_directory: bool,
    is_hidden: bool,
}

impl FileEntry {
    pub fn new(path: PathBuf) -> Self {
        let name = path.file_name().unwrap().to_string_lossy().into_owned();
        let emoji = get_emoji(&path);
        let is_directory = path.is_dir();
        let is_hidden = name.starts_with('.');
        
        FileEntry { emoji, name, path, is_directory, is_hidden }
    }

    fn get_extension(&self) -> Option<String> {
        self.path.extension().and_then(|e| e.to_str()).map(|s| s.to_lowercase())
    }
}

impl Ord for FileEntry {
    fn cmp(&self, other: &Self) -> Ordering {
        // First, sort directories before files
        match (self.is_directory, other.is_directory) {
            (true, false) => return Ordering::Less,
            (false, true) => return Ordering::Greater,
            _ => {}
        }

        // Then, sort hidden files and directories
        match (self.is_hidden, other.is_hidden) {
            (true, false) => return Ordering::Less,
            (false, true) => return Ordering::Greater,
            _ => {}
        }

        // For files, sort by extension first
        if !self.is_directory && !other.is_directory {
            match (self.get_extension(), other.get_extension()) {
                (Some(a), Some(b)) if a != b => return a.cmp(&b),
                _ => {}
            }
        }

        // Finally, sort by name (case-insensitive)
        self.name.to_lowercase().cmp(&other.name.to_lowercase())
    }
}

impl PartialOrd for FileEntry {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl PartialEq for FileEntry {
    fn eq(&self, other: &Self) -> bool {
        self.name.to_lowercase() == other.name.to_lowercase()
    }
}

impl Eq for FileEntry {}
