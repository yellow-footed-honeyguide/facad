use std::path::Path;
use std::fs;
use std::fs::File;
use std::io::{Read, BufReader};


#[cfg(unix)]  // Use conditional compilation for unix specific API
// Use lifetime paramter to indicate that the `default` will be valid after it is returned
fn get_unix_emoji_or<'a>(path: &Path, default: &'a str) -> &'a str {
    use std::os::unix::fs::FileTypeExt;
    let filename = path.file_name().unwrap_or_default();
    let filename_str: &str = filename.to_str().unwrap_or_default();
    let err_msg = format!("Failed to get metadata for path {}", filename_str);

    let metadata = path.metadata().expect(&err_msg);
    let file_type = metadata.file_type();
    let emoji = if file_type.is_fifo() { "⏩" }
    else if file_type.is_socket() { "󰟩" }
    else if file_type.is_char_device() { "🔤" }
    else if file_type.is_block_device() { "💽" }
    else { default };
    return emoji;
}

// Returns appropriate emoji for given file path
pub fn get_emoji(path: &Path) -> String {
    let common_default = "❓";
    let default = if cfg!(unix) {
        get_unix_emoji_or(path, common_default)
    } else {
        common_default
    };

    if path.is_symlink() {
        return if path.is_dir() { "🔗📁".to_string() } else { "🔗".to_string() };
    }
    if path.is_dir() {
        return "📁".to_string();
    }
    
    // Special handling for /dev directory
    if let Some(parent) = path.parent() {
        if parent == Path::new("/dev") {
            return get_dev_emoji(path);
        }
    }

    let extension = path.extension().and_then(|e| e.to_str()).unwrap_or("");
    let file_name = path.file_name().and_then(|n| n.to_str()).unwrap_or("");
        
    match extension {
        "md" => "📑",
        "jpg" | "jpeg" | "png" | "gif" | "bmp" | "svg" | "webp" => "📸",
        "mp4" | "avi" | "mkv" | "mov" | "flv" | "wmv" | "webm" => "🎬",
        "mp3" | "wav" | "ogg" | "flac" | "m4a" | "aac" => "🎧",
        "zip" | "tar" | "gz" | "bz2" | "xz" | "7z" | "rar" => "📦",
        "deb" | "rpm" => "📥",
        "py" | "sh" | "js" | "html" | "css" | "cpp" | "c" | "java" |
        "go" | "rb" | "rs" | "php" | "h" | "hpp" => "💻",
        "o" => "🧩",
        "txt" | "rst" | "log" => "📝",
        "ttf" | "otf" | "woff" | "woff2" => "🔤",
        "pdf" | "djvu" | "epub" => "📚",
        ".pem" | ".crt" | ".key" | ".pub" | ".p12" => "🔑",
        "csv" => "📊",
        "torrent" => "🌊",
        "iso" | "img" => "💽",
        "qcow" | "qcow2" => "🐮",
        "vv" => "🕹️",
        "doc" | "docx" | "odt" | "rtf" | "xls" | "xlsx" |
        "ods" | "ppt" | "pptx" | "odp" => "📄",
        "conf" | "config" | "toml" | "cfg" | "yaml" | "yml" | "json" | "ini" => "⚙️",
        _ => {
            if file_name.starts_with('.') {
                "⚙️"
            } else if is_executable(path) {
                "💾"
            } else if is_text_file(path) {
                "📝"
            } else {
                default
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


fn get_dev_emoji(path: &Path) -> String {
    let file_name = path.file_name().and_then(|n| n.to_str()).unwrap_or("");
    match file_name {
        "null" | "zero" => "⓿".to_string(),
        "random" | "urandom" => "🎲".to_string(),
        s if s.starts_with("tty") => "🖥️".to_string(),
        s if s.starts_with("sd") => "💽".to_string(),
        s if s.starts_with("nvme") => "💽".to_string(),
        s if s.starts_with("loop") => "🔁".to_string(),
        s if s.starts_with("usb") => "🔌".to_string(),
        _ => "🔧".to_string() // Default device emoji
    }
}


