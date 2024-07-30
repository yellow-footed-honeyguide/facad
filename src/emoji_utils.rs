use std::path::Path;
use std::fs;

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
        "jpg" | "jpeg" | "png" | "gif" | "bmp" | "svg" | "webp" => "🎨",
        "mp4" | "avi" | "mkv" | "mov" | "flv" | "wmv" | "webm" => "🎬",
        "mp3" | "wav" | "ogg" | "flac" | "m4a" | "aac" => "🎧",
        "ppt" | "pptx" => "📽️",
        "conf" | "config" | "cfg" | "ini" | "yaml" | "yml" | "json" | "xml" => "⚙️",
        _ => {
            if file_name.starts_with('.') {
                "⚙️"
            } else if is_executable(path) {
                "🚀"
            } else {
                "📰"
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
