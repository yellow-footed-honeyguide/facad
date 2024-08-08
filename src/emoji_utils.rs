use std::path::Path;
use std::fs;
use std::fs::File;
use std::io::{Read, BufReader};

// Returns appropriate emoji for given file path
pub fn get_emoji(path: &Path) -> String {
    if path.is_symlink() {
        return if path.is_dir() { "🔗📁".to_string() } else { "🔗".to_string() };
    }
    if path.is_dir() {
        return "📁".to_string();
    }
    let extension = path.extension().and_then(|e| e.to_str()).unwrap_or("");
    let file_name = path.file_name().and_then(|n| n.to_str()).unwrap_or("");
        
    match extension {
        "txt" => "📝",
        "md" => "📑",
        "jpg" | "jpeg" | "png" | "gif" | "bmp" | "svg" | "webp" => "📸",
        "mp4" | "avi" | "mkv" | "mov" | "flv" | "wmv" | "webm" => "🎬",
        "mp3" | "wav" | "ogg" | "flac" | "m4a" | "aac" => "🎧",
        "zip" | "tar" | "gz" | "bz2" | "xz" | "7z" | "rar" => "📦",
        "deb" | "rpm" => "📥",
        "py" | "sh" | "js" | "html" | "css" | "cpp" | "c" | "java" |
        "go" | "rb" | "rs" | "php" | "h" | "hpp" => "💻",
        "o" => "🧩",
        "txt" | "md" | "rst" | "log" => "📝",
        "ttf" | "otf" | "woff" | "woff2" => "🔤",
        "pdf" | "djvu" | "epub" => "📚",
        ".pem" | ".crt" | ".key" | ".pub" | ".p12" => "🔑",
        "csv" => "📊",
        "torrent" => "🌊",
        "iso" | "img" => "💽",
        "qcow" | "qcow2" => "🐮",
        "vv" => "🕹️",
        "doc" | "docx" | "odt" | "rtf" | "txt" | "pdf" | "xls" | "xlsx" |
        "ods" | "csv" | "ppt" | "pptx" | "odp" => "📄",
        "conf" | "config" | "toml" | "cfg" | "ini" | "yaml" | "yml" | "json" | "ini" => "⚙️",
        _ => {
            if file_name.starts_with('.') {
                "⚙️"
            } else if is_executable(path) {
                "💾"
            } else if is_text_file(path) {
                "📝"
            } else {
                "❓"
            }
        } 
  }.to_string()
}

// Checks if the file is executable
pub fn is_executable(path: &Path) -> bool {
    if cfg!(unix) {
        use std::os::unix::fs::PermissionsExt;
        fs::metadata(path).map(|m| m.permissions().mode() & 0o111 != 0).unwrap_or(false)
    } else {
        path.extension().and_then(|e| e.to_str()) == Some("exe")
    }
}

fn is_text_file(path: &Path) -> bool {
    if let Ok(file) = File::open(path) {
        let mut reader = BufReader::new(file);
        let mut buffer = [0u8; 1024];
        if let Ok(size) = reader.read(&mut buffer) {
            // Check if the file is empty
            if size == 0 {
                return true;
            }
            // Check if the file contains only printable ASCII characters and common whitespace
            return buffer[..size].iter().all(|&byte| byte.is_ascii_graphic() || byte.is_ascii_whitespace());
        }
    }
    false
}
